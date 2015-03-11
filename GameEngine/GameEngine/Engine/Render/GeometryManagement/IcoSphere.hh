#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <cstdint>

namespace AGE
{
	void generateIcoSphere(uint32_t recursion, std::vector<glm::vec3> &vertex, std::vector<glm::u32vec3> &indices);
}
