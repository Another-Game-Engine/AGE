#pragma once

#include <glm/gtc/quaternion.hpp>
#include <Core/PrepareKey.hpp>

namespace AGE
{
	struct PreparableObject
	{
		bool		 hasMoved;
		PrepareKey	 key;
		//glm::vec3	 position;
		//glm::vec3	 forward;
		//glm::vec3	 scale;
		//glm::quat	orientation;
		glm::mat4 transformation;
		PreparableObject();
		~PreparableObject();
	};
}