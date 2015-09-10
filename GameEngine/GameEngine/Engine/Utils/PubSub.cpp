#include "PubSub.hpp"

// PubSub

PubSub::PubSub(Manager *manager)
	: _manager(manager)
{}

PubSub::PubSub(PubSub &&o)
{
	_callbacks = std::move(o._callbacks);
	_subscribers = std::move(o._subscribers);
	_emitters = std::move(o._emitters);
	_manager = std::move(o._manager);
}

PubSub::~PubSub()
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

void PubSub::unsub(const PubSubKey &&key)
{
	if (_callbacks.find(key) != std::end(_callbacks))
	{
		delete static_cast<std::function<void()>*>(_callbacks[key].function);
		_callbacks.erase(key);
	}
	removeFromGlobalCallbacks(key);
}

void PubSub::unsubAll()
{
	removeFromGlobalCallbacks();
	for (auto &e : _callbacks)
	{
		delete static_cast<std::function<void()>*>(e.second.function);
	}
	_callbacks.clear();
}

void PubSub::removeFromGlobalCallbacks()
{
	auto &collection = _manager->getCollection();
	for (auto &e : _callbacks)
	{
		if (collection.find(e.first) == std::end(collection))
			continue;
		collection[e.first].erase(this);
	}
}

void PubSub::removeFromGlobalCallbacks(const PubSubKey &key)
{
	auto &collection = _manager->getCollection();
	if (collection.find(key) == std::end(collection))
		return;
	collection[key].erase(this);

}

void PubSub::_unsubAll()
{
	for (auto &e : _callbacks)
	{
		delete static_cast<std::function<void()>*>(e.second.function);
	}
	_callbacks.clear();
}


// End PubSub
// PubSub::Manager

PubSub::Manager::Manager()
{}

PubSub::Manager::~Manager()
{
	clearAll();
}

void PubSub::Manager::clearAll()
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

// End PubSub::Manager

