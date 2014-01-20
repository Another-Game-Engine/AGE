#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>

namespace	Component
{

	static int uniqueId()
	{
		static int id = 0;
		return id++;
	}

	struct Base
	{
		Base(Entity &entity);
		virtual ~Base();
		virtual Base &operator=(const Base &other);
		void			setEntity(Entity &entity);
		Entity		&getEntity();
		virtual void reset() = 0;
	protected:
		Entity				_entity;
	};

	template <class T>
	struct ComponentBase : public Base
	{
		ComponentBase(Entity &entity)
		: Base(entity)
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