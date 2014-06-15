#pragma once

namespace AGE
{
	struct Bone
	{
		std::string name;
		glm::mat4 offset;
		unsigned int index;
		glm::mat4 transformation;
		std::vector<unsigned int> children;
		unsigned int parent = (unsigned int)(-1);
		std::vector<std::vector<AnimationChannel*>> animations;
	};
}