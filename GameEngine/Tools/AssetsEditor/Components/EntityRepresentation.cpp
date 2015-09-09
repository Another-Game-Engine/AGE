#include "EntityRepresentation.hpp"
#include <Core/AScene.hh>
#include <Managers/ArchetypesEditorManager.hpp>

namespace AGE
{
	namespace WE
	{
		EntityRepresentation::EntityRepresentation()
		{
			editorOnly = false;
		}

		EntityRepresentation::~EntityRepresentation()
		{

		}

		void EntityRepresentation::_copyFrom(const ComponentBase *model)
		{
			auto o = static_cast<const EntityRepresentation*>(model);
			editorOnly = o->editorOnly;
			auto copyName = std::string(o->name);
			copyName += "-Copy";
			auto len = copyName.size();
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(name, copyName.c_str(), len);
			auto &link = entity->getLink();
			position = link.getPosition();
			rotation = glm::eulerAngles(link.getOrientation());
			scale = link.getScale();
		}

		void EntityRepresentation::init(const char *_name, const std::string &layerName /*= ""*/)
		{
			editorOnly = false;
			setName(_name);
			auto &link = entity->getLink();
			position = link.getPosition();
			rotation = glm::eulerAngles(link.getOrientation());
			scale = link.getScale();
		}

		void EntityRepresentation::reset()
		{
			if (_archetypeLinked != nullptr)
			{
				_archetypeLinked->entities.erase(entity);
			}
			memset(name, 0, ENTITY_NAME_LENGTH);
		}

		void EntityRepresentation::postUnserialization()
		{
			position = entity->getLink().getPosition();
			rotation = glm::eulerAngles(entity->getLink().getOrientation());
			scale = entity->getLink().getScale();
			if (strlen(name) == 0)
			{
				memcpy(name, "NoName", strlen("NoName"));
			}
		}

		void EntityRepresentation::setName(const char *_name)
		{
			auto len = strlen(_name);
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(name, _name, len);
		}
	}
}