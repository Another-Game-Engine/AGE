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

		void EntityRepresentation::init(AScene *s, const char *_name)
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
		}

		void EntityRepresentation::reset(AScene *)
		{
			memset(name, 0, ENTITY_NAME_LENGTH);
		}

		EntityRepresentation &EntityRepresentation::operator=(EntityRepresentation &&o)
		{
			memcpy(name, o.name, ENTITY_NAME_LENGTH);
			position = std::move(o.position);
			rotation = std::move(o.rotation);
			scale = std::move(o.scale);
			return *this;
		}

		EntityRepresentation::EntityRepresentation(EntityRepresentation &&o)
			: ComponentBase(std::move(o))
		{
			memcpy(name, o.name, ENTITY_NAME_LENGTH);
			position = std::move(o.position);
			rotation = std::move(o.rotation);
			scale = std::move(o.scale);
		}

		void EntityRepresentation::postUnserialization(AScene *scene)
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