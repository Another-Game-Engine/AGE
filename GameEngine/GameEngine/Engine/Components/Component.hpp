#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>
#include <Entities/Entity.hh>

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
		Base(Engine &engine, const std::string &name = "NoName")
		: _engine(engine)
		, _name(name)
		, _entity(Handle(std::numeric_limits<unsigned int>::max(), nullptr))
		{
		}

		virtual ~Base(){}

		virtual Base &operator=(const Base &other)
		{
			return *this;
		}

		void			setEntity(Handle &entity)
		{
			_entity = entity;
		}

		Handle		&getEntity()
		{
			return _entity;
		}

		inline std::string const &getName() const
		{
			return _name;
		}

	protected:
		Engine              &_engine;
		std::string         _name;
		Handle				_entity;
	};

	template <class T>
	struct ComponentBase : public Base
	{
		virtual ~ComponentBase()
		{}
		ComponentBase(Engine &engine, const std::string &name = "NoName")
		: Base(engine, name)
		{}

		static unsigned int getTypeId()
		{
			static unsigned int id = uniqueId();
			return id;
		}
	};
}

#endif