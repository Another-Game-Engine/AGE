#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Core/PrepareKey.hpp>
#include <Utils/AABoundingBox.hh>

namespace AGE
{

	struct PreparableObject
	{
		bool active;
		bool hasMoved;
		PrepareKey key;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 scale;
		glm::quat orientation;
		PreparableObject();
		virtual ~PreparableObject();
	};

}