#ifndef   __PUBSUB_HPP__
# define  __PUBSUB_HPP__

#include <algorithm>
#include <unordered_set>
#include <map>
#include <memory>
#include <functional>
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

	class Manager
	{
	public:
		template <typename ...Args>
		void pub(PubSubKey &name, Args ...args)
		{
			auto set = _collection.find(name);
			if (set == std::end(_collection) || set->second.empty())
				return;
			for (auto it = std::begin(set->second); it != std::end(set->second);)
			{
				auto e = *it;
				++it;
				e->call(name, args...);
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

		~Manager()
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
	void globalSub(const PubSubKey &key, F lambda)
	{
		auto &collection = _manager.getCollection();
		if (_callbacks.find(key) != std::end(_callbacks))
			return;
		auto fn = new decltype(toFn(lambda))(toFn(lambda));
		_callbacks[key].function = static_cast<void*>(fn);
		_callbacks[key].signature = &typeid(fn);
		if (collection.find(key) == std::end(collection))
			collection.emplace(key, std::unordered_set<PubSub*>());
		collection[key].insert(this);
	}

	void unsub(const PubSubKey &key)
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
	void broadCast(PubSubKey &name, Args ...args)
	{
		_manager.pub(name, args...);
	}

	template <typename ...Args>
	void call(PubSubKey &name, Args ...args)
	{
		auto callback = _callbacks.at(name);
		auto function = static_cast<std::function<void(Args...)>*>(callback.function);

		if (typeid(function) != *(callback.signature))
		{
			std::cerr << "PubSub error : Wrong pub call." << std::endl;
			return;
		}
		(*function)(args...);
	}

	PubSub(Manager &manager)
		: _manager(manager)
	{}

	virtual ~PubSub()
	{
		unsubAll();
	}

	Manager &getPubSubManager()
	{
		return _manager;
	}
private:
	void removeFromGlobalCallbacks()
	{
		auto &collection = _manager.getCollection();
		for (auto &e : _callbacks)
		{
			if (collection.find(e.first) == std::end(collection))
				continue;
			collection[e.first].erase(this);
		}
	}

	void removeFromGlobalCallbacks(const PubSubKey &key)
	{
		auto &collection = _manager.getCollection();
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
	Manager &_manager;
};

#endif    //__PUBSUB_HPP__