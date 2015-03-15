#include "EntityRepresentation.hpp"
#include <Core/AScene.hh>


namespace AGE
{
	namespace WE
	{
		EntityRepresentation::EntityRepresentation()
		{
#ifdef EDITOR_ENABLED
			exposedInEditor = false;
#endif
		}

		EntityRepresentation::~EntityRepresentation()
		{

		}

		void EntityRepresentation::init(const char *_name)
		{
#ifdef EDITOR_ENABLED
			exposedInEditor = false;
#endif
			auto len = strlen(_name);
			if (len >= ENTITY_NAME_LENGTH)
				len = ENTITY_NAME_LENGTH;
			memcpy(name, _name, len);
			auto &link = entity.getLink();
			position = link.getPosition();
			rotation = glm::eulerAngles(link.getOrientation());
			scale = link.getScale();
			layer = 0;
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