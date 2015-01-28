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
				for (auto i = 0; i < name.size() && name[i]; ++i)
					name[i] = _name[i];
			}
			auto link = s->getLink(this->entityId);
			position = link->getPosition();
			rotation = glm::eulerAngles(link->getOrientation());
			scale = link->getScale();
		}

		void EntityRepresentation::reset(AScene *)
		{
			name.fill('\0');
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