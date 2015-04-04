#pragma once

#include <Entities/Entity.hh>
#include <Core/AScene.hh>

namespace AGE
{
	class Archetype
	{
	public:
		static bool Load(const std::string &path, AScene *scene);
		static std::shared_ptr<Archetype> &Create();

		~Archetype(){}

		Entity spawn(AScene *scene)
		{
			// todo
			return scene->createEntity();
		}

		bool save(const std::string &path)
		{
			//todo
			return true;
		}

		void setName(const std::string &name)
		{
			_name = name;
		}

		const std::string &getName() const
		{
			return _name;
		}

		Entity &getEntity()
		{
			return _entity;
		}
	private:
		Entity _entity;
		std::string _name;
	};
}