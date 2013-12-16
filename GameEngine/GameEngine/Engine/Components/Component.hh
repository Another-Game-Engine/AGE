#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>
#include <Entities/Handle.hh>

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
		Base(Engine &engine, const std::string &name = "NoName");
		virtual ~Base();
		virtual Base &operator=(const Base &other);
		void			setEntity(Handle &entity);
		Handle		&getEntity();
		std::string const &getName() const;
	protected:
		Engine              &_engine;
		std::string         _name;
		Handle				_entity;
	};

	template <class T>
	struct ComponentBase : public Base
	{
		ComponentBase(Engine &engine, const std::string &name)
			: Base(engine, name)
		{}

		virtual ~ComponentBase()
		{}

		static unsigned int getTypeId()
		{
			static unsigned int id = uniqueId();
			return id;
		}
		//ComponentBase(Engine &engine, const std::string &name = "NoName");
		//virtual ~ComponentBase();
		//static unsigned int getTypeId();
	};
}

#endif