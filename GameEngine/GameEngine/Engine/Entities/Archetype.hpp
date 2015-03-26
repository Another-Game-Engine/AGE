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

		~Archetype();

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

		void update(AScene *scene)
		{
			if (!_entityModel.isValid())
			{
				return;
			}
			if (!_archetype.isValid())
			{
				_archetype = scene->createEntity(true);
			}
			scene->copyEntity(_entityModel, _archetype, true, true);
		}
	private:
		Archetype();
		Archetype(const Archetype &);
		Entity _entityModel;
		Entity _archetype;
		std::string _name;
	};
}