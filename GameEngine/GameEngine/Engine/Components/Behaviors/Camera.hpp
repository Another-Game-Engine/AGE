#pragma once

#include <Entities/EntityTypedef.hpp>
#include <cstddef>

#include <Utils/Frustum.hpp>
#include <glm/glm.hpp>
#include <Core/Octree.hpp>

class AScene;

namespace AGE
{
	namespace ComponentBehavior
	{
		struct Camera
		{
		private:
			Octree::CAMERA_ID _cameraId;
		public:
			void init(::AScene *scene, ENTITY_ID entityId);
			void reset(::AScene *scene, ENTITY_ID entityId);

			//void setLookAtTransform(const glm::mat4 &lookAt);
			//const glm::mat4& getLookAtTransform() const;
			void setProjection(const glm::mat4 &);
			const glm::mat4 &getProjection() const;
		private:
			glm::mat4 _projection;
		};
	}
}