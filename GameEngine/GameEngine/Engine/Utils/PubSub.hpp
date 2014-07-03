#ifndef   __PUBSUB_HPP__
# define  __PUBSUB_HPP__

#include <algorithm>
#include <unordered_set>
#include <map>
#include <memory>
#include <functional>
#include <Utils/Dependency.hpp>
#include  "Function.hpp"

typedef std::string PubSubKey;

class PubSub
{
public:
	struct Callback
	{
		void *function;
		const std::type_info *signature;
	};

public:

	// manager class
	//
	//

	class Manager : public Dependency<Manager>
	{
	public:
		template <typename ...Args>
		void pub(const PubSubKey &&name, Args ...args) const
		{
			auto set = _collection.find(name);
			if (set == std::end(_collection) || set->second.empty())
				return;
			for (auto it = std::begin(set->second); it != std::end(set->second);)
			{
				auto e = *it;
				++it;
				e->call(std::move(name), args...);
			}
		}

		void clearAll()
		{
			for (auto &e : _collection)
			{
				for (auto &o : e.second)
				{
					o->_unsubAll();
				}
				e.second.clear();
			}
		}
		
		std::map<PubSubKey, std::unordered_set<PubSub*> > &getCollection()
		{
			return _collection;
		}

		Manager()
		{}

		virtual ~Manager()
		{
			clearAll();
		}
	private:
		std::map<PubSubKey, std::unordered_set<PubSub*> > _collection;
		Manager(const Manager &o);
		Manager &operator=(const Manager &o);
	};

	//
	//
	// End of manager class

public:
	template <typename F>
	void globalSub(const PubSubKey &&key, F lambda)
	{
		auto &collection = _manager->getCollection();
		if (_callbacks.find(key) != std::end(_callbacks))
			return;
		auto fn = new decltype(toFn(lambda))(toFn(lambda));
		_callbacks[key].function = static_cast<void*>(fn);
		_callbacks[key].signature = &typeid(fn);
		if (collection.find(key) == std::end(collection))
			collection.emplace(key, std::unordered_set<PubSub*>());
		collection[key].insert(this);
	}

	template <typename F>
	void sub(const PubSubKey &&key, PubSub *emitter, F lambda)
	{
		auto &collection = emitter->getSubscribers();
		if (_callbacks.find(key) != std::end(_callbacks))
			return;
		auto fn = new decltype(toFn(lambda))(toFn(lambda));
		_callbacks[key].function = static_cast<void*>(fn);
		_callbacks[key].signature = &typeid(fn);
		if (collection.find(key) == std::end(collection))
			collection.emplace(key, std::unordered_set<PubSub*>());
		collection[key].insert(this);
		_emitters.insert(emitter);
	}

	void unsub(const PubSubKey &&key)
	{
		if (_callbacks.find(key) != std::end(_callbacks))
		{
			delete static_cast<std::function<void()>*>(_callbacks[key].function);
			_callbacks.erase(key);
		}
		removeFromGlobalCallbacks(key);
	}

	void unsubAll()
	{
		removeFromGlobalCallbacks();
		for (auto &e : _callbacks)
		{
			delete static_cast<std::function<void()>*>(e.second.function);
		}
		_callbacks.clear();
	}
	
	template <typename ...Args>
	void broadCast(const PubSubKey &&name, Args ...args) const
	{
		_manager->pub(std::move(name), args...);
	}

	template <typename ...Args>
	void pub(const PubSubKey &&name, Args ...args) const
	{
		auto set = _subscribers.find(name);
		if (set == std::end(_subscribers) || set->second.empty())
			return;
		for (auto it = std::begin(set->second); it != std::end(set->second);)
		{
			auto e = *it;
			++it;
			e->call(name, args...);
		}
	}

	template <typename ...Args>
	void call(const PubSubKey &&name, Args ...args)
	{
		auto callback = _callbacks.at(name);
		auto function = static_cast<std::function<void(Args...)>*>(callback.function);

		if (typeid(function) != *(callback.signature))
		{
			std::cerr << "PubSub error : Wrong pub call. the key is " + name << std::endl;
			return;
		}
		(*function)(args...);
	}

	PubSub(Manager *manager)
		: _manager(manager)
	{}

	PubSub(PubSub &&o)
	{
		_callbacks = std::move(o._callbacks);
		_subscribers = std::move(o._subscribers);
		_emitters = std::move(o._emitters);
		_manager = std::move(o._manager);
	}

	virtual ~PubSub()
	{
		// unsub local subscriber
		for (auto &k : _subscribers)
		{
			for (auto &e : k.second)
			{
				e->_emitters.erase(this);
			}
			k.second.clear();
		}
		_subscribers.clear();

		// remove this from emitters list
		for (auto &e : _emitters)
		{
			for (auto &c : _callbacks)
			{
				auto &subscription = e->_subscribers.find(c.first);
				if (subscription == std::end(e->_subscribers))
					continue;
				subscription->second.erase(this);
			}
		}
		unsubAll();
	}

	Manager *getPubSubManager()
	{
		return _manager;
	}

	std::map<PubSubKey, std::unordered_set<PubSub*> > &getSubscribers()
	{
		return _subscribers;
	}
private:
	void removeFromGlobalCallbacks()
	{
		auto &collection = _manager->getCollection();
		for (auto &e : _callbacks)
		{
			if (collection.find(e.first) == std::end(collection))
				continue;
			collection[e.first].erase(this);
		}
	}

	void removeFromGlobalCallbacks(const PubSubKey &key)
	{
		auto &collection = _manager->getCollection();
		if (collection.find(key) == std::end(collection))
			return;
		collection[key].erase(this);

	}

	void _unsubAll()
	{
		for (auto &e : _callbacks)
		{
			delete static_cast<std::function<void()>*>(e.second.function);
		}
		_callbacks.clear();
	}

	std::map<PubSubKey, Callback> _callbacks;
	std::map<PubSubKey, std::unordered_set<PubSub*> > _subscribers;
	std::unordered_set<PubSub*> _emitters;
	Manager *_manager;
};

#endif    //__PUBSUB_HPP__