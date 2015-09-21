#pragma once

#include "Entity/Entity.hh"

namespace AGE
{
	namespace Physics
	{
		namespace Private
		{
			struct GenericData
			{
				// Attributes
				void *data = nullptr;

				Entity entity;
			};
		}
	}
}