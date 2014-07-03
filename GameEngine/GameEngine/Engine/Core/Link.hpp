#pragma once

#include <glm/glm.hpp>
#include <Utils/BoundingInfos.hpp>
#include <Entities/EntityTypedef.hpp>

namespace AGE
{
	struct Link
	{
		glm::vec3 _position;
		glm::vec3 _scale;
		glm::quat _orientation;
		glm::mat4 _transformation;
		ENTITY_ID _entityID;
		BoundingInfos _boundingInfos;
		
	};
}