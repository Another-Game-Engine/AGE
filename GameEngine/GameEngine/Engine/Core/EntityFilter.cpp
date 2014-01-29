#include <Core/EntityFilter.hpp>

bool defaultEntityComparaison(Entity e1, Entity e2)
{
	return e1 < e2;
}