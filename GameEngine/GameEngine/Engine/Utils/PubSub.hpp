#ifndef   __PUBSUB_HPP__
# define  __PUBSUB_HPP__

#include <memory>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <map>
#include "Singleton.hh"

template <typename F>
struct MyFunc : public MyFunc<decltype(&F::operator())>
{};

template <typename Type, typename Return, typename... Args>
struct MyFunc<Return(Type::*)(Args...) const>
{
	typedef Return(*pointer)(Args...);
	typedef std::function<Return(Args...)> function;
};

template <typename F>
typename MyFunc<F>::pointer
toFnPointer(F &lambda)
{
	return static_cast<typename MyFunc<F>::pointer>(lambda);
}


template <typename F>
typename MyFunc<F>::function
toFn(F &lambda)
{
	return static_cast<typename MyFunc<F>::function>(lambda);
}

class PubSub
{
public:
	struct Callback
	{
		void *function;
		const std::type_info *signature;
	};

	struct PubSubManager
	{
	public:
		void clearAll()
		{
			for (auto &e : allCallbacks)
			{
				for (auto &o : e.second)
				{
					o->clearSubscriptions();
				}
				e.second.clear();
			}
		}

	template <typename ...Args>
	void broadcast(std::string name, Args ...args)
	{
		auto set = allCallbacks.find(name);
		if (set == std::end(allCallbacks) || set->second.empty())
			return;
		for (auto it = std::begin(set->second); it != std::end(set->second); )
		{
			auto e = *it;
			++it;
			e->pub(name, args...);
		}
	}

	void removeFromGlobalCallbacks(const std::map<std::string, Callback> callbacks, PubSub *ptr)
	{
		for (auto &e : callbacks)
		{
			if (allCallbacks.find(e.first) == std::end(allCallbacks))
				continue;
			allCallbacks[e.first].erase(ptr);
		}
	}

	void removeFromGlobalCallbacks(const std::string &key, PubSub *ptr)
	{
		if (allCallbacks.find(key) == std::end(allCallbacks))
			return;
		allCallbacks[key].erase(ptr);

	}

	std::map<std::string, std::unordered_set<PubSub*> > allCallbacks;
	};

	typedef Singleton<PubSubManager> Manager;

public:
	void clearSubscriptions()
	{
		for (auto &e : _callbacks)
		{
			delete static_cast<std::function<void()>*>(e.second.function);
		}
		_callbacks.clear();
	}

	void clearAll()
	{
		Manager::instance()->clearAll();
	}

	template <typename F>
	void sub(const std::string &key, F lambda)
	{
		if (_callbacks.find(key) != std::end(_callbacks))
			return;
		auto fn = new decltype(toFn(lambda))(toFn(lambda));
		_callbacks[key].function = static_cast<void*>(fn);
		_callbacks[key].signature = &typeid(fn);
		if (_manager->allCallbacks.find(key) == std::end(_manager->allCallbacks))
			_manager->allCallbacks.emplace(key, std::unordered_set<PubSub*>());
		_manager->allCallbacks[key].insert(this);
	}

	void unsub(const std::string &key)
	{
		if (_callbacks.find(key) != std::end(_callbacks))
		{
			delete static_cast<std::function<void()>*>(_callbacks[key].function);
			_callbacks.erase(key);
		}
		Manager::instance()->removeFromGlobalCallbacks(key, this);
	}

	template <typename ...Args>
	void pub(std::string name, Args ...args)
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

	PubSub()
		: _manager(Manager::instance())
	{}
	virtual ~PubSub()
	{
		Manager::instance()->removeFromGlobalCallbacks(_callbacks, this);
		clearSubscriptions();
	}

private:
	std::map<std::string, Callback> _callbacks;
	PubSubManager *_manager;
};

#endif    //__PUBSUB_HPP__