#pragma once

#include <Entities/EntityTypedef.hpp>
#include <cstddef>

#include <glm/glm.hpp>

class AScene;

namespace AGE
{
	namespace ComponentBehavior
	{
		struct Camera
		{
		private:
			std::size_t _cameraId;
		public:
			void init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId);
			void reset(::AScene *scene, ENTITY_ID entityId);

			//TEMPORARY FOR TEST
			// have to be passed as private
		public:
			glm::mat4 lookAtTransform;
			glm::mat4 projection;
		};
	}
}