#pragma once

#include <Core/PrepareKey.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AGE
{

	struct PreparableObject
	{
		bool hasMoved;
		bool active;
		PrepareKey key;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 scale;
		glm::quat orientation;
		PreparableObject();
		virtual ~PreparableObject();
	};

}