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
			if (_name)
			{
				auto i = 0;
				for (; i < name.size() - 1 && _name[i] != '\0'; ++i)
					name[i] = _name[i];
				name[i] = '\0';
			}
			auto link = s->getLink(this->entity);
			position = link->getPosition();
			rotation = glm::eulerAngles(link->getOrientation());
			scale = link->getScale();
		}

		void EntityRepresentation::reset(AScene *)
		{
			name.fill(0);
		}

		EntityRepresentation &EntityRepresentation::operator=(EntityRepresentation &&o)
		{
 			name = std::move(o.name);
			position = std::move(o.position);
			rotation = std::move(o.rotation);
			scale = std::move(o.scale);
			return *this;
		}

		EntityRepresentation::EntityRepresentation(EntityRepresentation &&o)
			: ComponentBase<EntityRepresentation>(std::move(o))
		{
			name = std::move(o.name);
			position = std::move(o.position);
			rotation = std::move(o.rotation);
			scale = std::move(o.scale);
		}
	}
}