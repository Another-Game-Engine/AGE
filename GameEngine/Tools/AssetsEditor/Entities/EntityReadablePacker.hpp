#pragma once

#include <vector>

namespace AGE
{
	class Entity;
	struct ReadableEntityPack;

	static void CreateReadableEntityPack(ReadableEntityPack &pack, std::vector<Entity> &selection);
}