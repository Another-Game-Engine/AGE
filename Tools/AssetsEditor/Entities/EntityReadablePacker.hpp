#pragma once

#include <vector>

namespace AGE
{
	class Entity;
	struct ReadableEntityPack;
	struct ReadableEntity;

	void EntityToFlatReadableVector(std::vector<ReadableEntity> &vector, const Entity &e);
	void SelectionToFlatReadableVector(const std::vector<Entity> &selection, std::vector<ReadableEntity> &result);
	void CreateReadableEntityPack(ReadableEntityPack &pack, std::vector<Entity> &selection);
	void CreateReadableEntityPack(ReadableEntityPack &pack, Entity &selection);
}