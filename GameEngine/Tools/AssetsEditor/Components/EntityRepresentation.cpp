#include "EntityRepresentation.hpp"
#include <Core/AScene.hh>


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