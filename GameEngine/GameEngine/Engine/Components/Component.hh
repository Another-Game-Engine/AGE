#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>
#include <Entities/Entity.hh>
#include <Utils/PubSub.hpp>
#include <Core/AScene.hh>

class AScene;

namespace	Component
{

	static int uniqueId()
	{
		static int id = 0;
		return id++;
	}

	struct Base
	{
		Base(AScene *scene, Entity &entity, const std::string &name = "NoName");
		virtual ~Base();
		virtual Base &operator=(const Base &other);
		void			setEntity(Entity &entity);
		Entity		&getEntity();
		std::string const &getName() const;
		virtual void reset() = 0;
	protected:
		AScene              *_scene;
		std::string         _name;
		Entity				_entity;
	};

	template <class T>
	struct ComponentBase : public Base, public PubSub
	{
		ComponentBase(AScene *scene, Entity &entity, const std::string &name = "DefaultComponentName")
		: Base(scene, entity, name),
		PubSub(scene->getInstance<PubSub::Manager>())
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