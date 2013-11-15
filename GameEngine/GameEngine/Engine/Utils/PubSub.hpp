#ifndef   __PUBSUB_HPP__
# define  __PUBSUB_HPP__

#include <memory>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <map>

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
public:
	void clearSubscriptions()
	{
		for (auto &e : _callbacks)
		{
			delete static_cast<std::function<void()>*>(e.second.function);
		}
		_callbacks.clear();
	}

	static void clearAll()
	{
		for (auto &e : _allCallbacks)
		{
			for (auto &o : e.second)
			{
				o->clearSubscriptions();
			}
			e.second.clear();
		}
	}

	template <typename F>
	void sub(const std::string &key, F lambda)
	{
		if (_callbacks.find(key) != std::end(_callbacks))
			return;
		auto fn = new decltype(toFn(lambda))(toFn(lambda));
		_callbacks[key].function = static_cast<void*>(fn);
		_callbacks[key].signature = &typeid(fn);
		if (_allCallbacks.find(key) == std::end(_allCallbacks))
			_allCallbacks.emplace(key, std::unordered_set<PubSub*>());
		_allCallbacks[key].insert(this);
	}

	void unsub(const std::string &key)
	{
		if (_callbacks.find(key) != std::end(_callbacks))
		{
			delete static_cast<std::function<void()>*>(_callbacks[key].function);
			_callbacks.erase(key);
		}
		removeFromGlobalCallbacks(key);
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

	template <typename ...Args>
	static void broadcast(std::string name, Args ...args)
	{
		auto set = _allCallbacks.find(name);
		if (set == std::end(_allCallbacks) || set->second.empty())
			return;
		for (auto it = std::begin(set->second); it != std::end(set->second); )
		{
			auto e = *it;
			++it;
			e->pub(name, args...);
		}
	}

	PubSub(){}
	virtual ~PubSub()
	{
		removeFromGlobalCallbacks();
		clearSubscriptions();
	}

	static std::map<std::string, std::unordered_set<PubSub*> > _allCallbacks;
private:
	void removeFromGlobalCallbacks()
	{
		for (auto &e : _callbacks)
		{
			if (_allCallbacks.find(e.first) == std::end(_allCallbacks))
				continue;
			_allCallbacks[e.first].erase(this);
		}
	}

	void removeFromGlobalCallbacks(const std::string &key)
	{
		if (_allCallbacks.find(key) == std::end(_allCallbacks))
			return;
		_allCallbacks[key].erase(this);

	}
	std::map<std::string, Callback> _callbacks;
};

#endif    //__PUBSUB_HPP__