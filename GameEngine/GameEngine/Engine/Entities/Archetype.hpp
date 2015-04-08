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

		Archetype()
		{
			auto name = "NoNameArchetype";
			auto len = strlen(name);
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(_name, name, len);
		}
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
			auto len = name.length();
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(_name, name.c_str(), len);
		}

		const char *getName() const
		{
			return _name;
		}

		char *getNamePtr()
		{
			return _name;
		}

		Entity &getEntity()
		{
			return _entity;
		}
	private:
		Entity _entity;
		char _name[ENTITY_NAME_LENGTH];
	};
}