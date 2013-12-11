#ifndef		__COMPONENT_HPP__
#define		__COMPONENT_HPP__

#include <string>

class	Entity;
class   Engine;

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
		, _father(nullptr)
		{
		}
		virtual ~Base(){}
		virtual Base &operator=(const Base &other)
		{
			return *this;
		}

		void			setFather(Entity *father)
		{
			_father = father;
		}

		inline Entity		*getFather() const
		{
			return _father;
		}

		inline std::string const &getName() const
		{
			return _name;
		}

	protected:
		Engine              &_engine;
		std::string         _name;
		Entity				*_father;
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