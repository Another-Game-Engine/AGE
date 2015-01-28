#include "EntityRepresentation.hpp"

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

		void EntityRepresentation::init(AScene *, const char *_name)
		{
			if (_name)
			{
				for (auto i = 0; i < name.size() && name[i]; ++i)
					name[i] = _name[i];
			}
		}

		void EntityRepresentation::reset(AScene *)
		{
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