#pragma once

#include <vector>

namespace AGE
{
	class Entity;
	struct ReadableEntityPack;

	void CreateReadableEntityPack(ReadableEntityPack &pack, std::vector<Entity> &selection);
	void CreateReadableEntityPack(ReadableEntityPack &pack, Entity &selection);
}