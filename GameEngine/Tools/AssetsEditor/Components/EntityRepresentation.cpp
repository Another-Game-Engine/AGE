#include "EntityRepresentation.hpp"
#include <Core/AScene.hh>

namespace AGE
{
	namespace WE
	{
		EntityRepresentation::EntityRepresentation()
		{

		}

		EntityRepresentation::~EntityRepresentation()
		{

		}

		void EntityRepresentation::init(AScene *s, const char *_name)
		{
			if (_name)
			{
				auto i = 0;
				for (; i < name.size() - 1 && _name[i] != '\0'; ++i)
					name[i] = _name[i];
				name[i] = '\0';
			}
			auto link = s->getLink(this->entityId);
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
			return *this;
		}

		EntityRepresentation::EntityRepresentation(EntityRepresentation &&o)
		{
			*this = std::move(o);
		}
	}
}