#ifndef   __PUBSUB_HPP__
# define  __PUBSUB_HPP__

#include <algorithm>
#include <unordered_set>
#include <map>
#include <memory>
#include <functional>

typedef std::string PubSubKey;

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

private:

	// manager class
	//
	//

	class Manager
	{
	public:
		std::map<PubSubKey, std::unordered_set<PubSub*> > collection;
		static Manager           &getInstance()
		{
			static Manager *instance = nullptr;
			if (instance == nullptr)
			{
				instance = new Manager();
			}
			return (*instance);
		}
	private:
		Manager(){}
		~Manager(){}
	};

	//
	//
	// End of manager class

public:
	template <typename F>
	void sub(const PubSubKey &key, F lambda)
	{
		if (_callbacks.find(key) != std::end(_callbacks))
			return;
		auto fn = new decltype(toFn(lambda))(toFn(lambda));
		_callbacks[key].function = static_cast<void*>(fn);
		_callbacks[key].signature = &typeid(fn);
		if (_manager.collection.find(key) == std::end(_manager.collection))
			_manager.collection.emplace(key, std::unordered_set<PubSub*>());
		_manager.collection[key].insert(this);
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
	void call(PubSubKey name, Args ...args)
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
	static void pub(PubSubKey name, Args ...args)
	{
		auto &col = Manager::getInstance().collection;
		auto set = col.find(name);
		if (set == std::end(col) || set->second.empty())
			return;
		for (auto it = std::begin(set->second); it != std::end(set->second); )
		{
			auto e = *it;
			++it;
			e->call(name, args...);
		}
	}

	static void clearAll()
	{
		auto &col = Manager::getInstance().collection;
		for (auto &e : col)
		{
			for (auto &o : e.second)
			{
				o->_unsubAll();
			}
			e.second.clear();
		}
	}

	PubSub()
		: _manager(Manager::getInstance())
	{}

	virtual ~PubSub()
	{
		unsubAll();
	}
private:
	void removeFromGlobalCallbacks()
	{
		for (auto &e : _callbacks)
		{
			if (_manager.collection.find(e.first) == std::end(_manager.collection))
				continue;
			_manager.collection[e.first].erase(this);
		}
	}

	void removeFromGlobalCallbacks(const PubSubKey &key)
	{
		if (_manager.collection.find(key) == std::end(_manager.collection))
			return;
		_manager.collection[key].erase(this);

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