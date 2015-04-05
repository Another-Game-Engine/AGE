#include "EntityRepresentation.hpp"
#include <Core/AScene.hh>
#include <Managers/ArchetypesEditorManager.hpp>

namespace AGE
{
	namespace WE
	{
		EntityRepresentation::EntityRepresentation()
		{
			exposedInEditor = false;
			editorOnly = false;
		}

		EntityRepresentation::~EntityRepresentation()
		{

		}

		void EntityRepresentation::_copyFrom(const ComponentBase *model)
		{
			auto o = static_cast<const EntityRepresentation*>(model);
			exposedInEditor = o->exposedInEditor;
			editorOnly = o->editorOnly;
			auto copyName = std::string(o->name);
			copyName += "-Copy";
			auto len = copyName.size();
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(name, copyName.c_str(), len);
			auto &link = entity.getLink();
			position = link.getPosition();
			rotation = glm::eulerAngles(link.getOrientation());
			scale = link.getScale();
		}

		void EntityRepresentation::init(const char *_name, const std::string &layerName /*= ""*/)
		{
			exposedInEditor = false;
			editorOnly = false;
			auto len = strlen(_name);
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(name, _name, len);
			auto &link = entity.getLink();
			position = link.getPosition();
			rotation = glm::eulerAngles(link.getOrientation());
			scale = link.getScale();
		}

		void EntityRepresentation::reset()
		{
			if (archetypeLinked != nullptr)
			{
				archetypeLinked->entities.erase(entity);
			}
			memset(name, 0, ENTITY_NAME_LENGTH);
		}

		void EntityRepresentation::postUnserialization()
		{
			position = entity.getLink().getPosition();
			rotation = glm::eulerAngles(entity.getLink().getOrientation());
			scale = entity.getLink().getScale();
			if (strlen(name) == 0)
			{
				memcpy(name, "NoName", strlen("NoName"));
			}
		}
	}
}