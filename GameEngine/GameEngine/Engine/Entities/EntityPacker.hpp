#pragma once

namespace AGE
{
	template <typename Container>
	static void EntityToFlatVector(std::vector<Container> &vector, const Entity &e)
	{
		SCOPE_profile_cpu_function("Entity packer");

		vector.push_back(Container());
		vector.back().entity = e;
		auto &children = e.getLink().getChildren();
		auto parentId = vector.size() - 1;
		for (auto &c : children)
		{
			vector[parentId].children.push_back(vector.size());
			auto child = c->getEntity()->getEntity();
			EntityToFlatVector(vector, child);
		}
	}

	template <typename Container>
	static void SelectionToFlatVector(const std::vector<Entity> &selection, std::vector<Container> &result)
	{
		SCOPE_profile_cpu_function("Entity packer");

		for (auto &e : selection)
		{
			EntityToFlatVector(result, e);
		}
	}
}