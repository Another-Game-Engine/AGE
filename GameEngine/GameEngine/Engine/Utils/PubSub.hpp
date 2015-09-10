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
		Manager();
		virtual ~Manager();
		void clearAll();

		inline std::map<PubSubKey, std::unordered_set<PubSub*> > &getCollection()
		{
			return _collection;
		}

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

	private:
		std::map<PubSubKey, std::unordered_set<PubSub*> > _collection;
		Manager(const Manager &o);
		Manager &operator=(const Manager &o);
	};

	//
	//
	// End of manager class

public:

	PubSub(Manager *manager);
	PubSub(PubSub &&o);
	virtual ~PubSub();

	void unsub(const PubSubKey &&key);
	void unsubAll();

	inline Manager *getPubSubManager()
	{
		return _manager;
	}

	inline std::map<PubSubKey, std::unordered_set<PubSub*> > &getSubscribers()
	{
		return _subscribers;
	}

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

private:
	void removeFromGlobalCallbacks();
	void removeFromGlobalCallbacks(const PubSubKey &key);
	void _unsubAll();

	std::map<PubSubKey, Callback> _callbacks;
	std::map<PubSubKey, std::unordered_set<PubSub*> > _subscribers;
	std::unordered_set<PubSub*> _emitters;
	Manager *_manager;
};

#endif    //__PUBSUB_HPP__