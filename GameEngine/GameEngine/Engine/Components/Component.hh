#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>
#include <Entities/Handle.hh>
#include <Utils/PubSub.hpp>

class Engine;

namespace	Component
{

	static int uniqueId()
	{
		static int id = 0;
		return id++;
	}

	struct Base
	{
		Base(Engine &engine, Handle &entity, const std::string &name = "NoName");
		virtual ~Base();
		virtual Base &operator=(const Base &other);
		void			setEntity(Handle &entity);
		Handle		&getEntity();
		std::string const &getName() const;
		virtual void reset() = 0;
	protected:
		Engine              &_engine;
		std::string         _name;
		Handle				_entity;
	};

	template <class T>
	struct ComponentBase : public Base, public PubSub
	{
		ComponentBase(Engine &engine, Handle &entity, const std::string &name = "DefaultComponentName")
		: Base(engine, entity, name),
		PubSub(entity->getPubSubManager())
		{}

		virtual ~ComponentBase()
		{
		}

		static unsigned int getTypeId()
		{
			static unsigned int id = uniqueId();
			return id;
		}
	private:
		//std::multiset<PubSubKey> _subscriptions; // subscriptions to local entity events
	};
}

#endif