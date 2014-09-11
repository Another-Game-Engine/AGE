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
		public:
			void init(::AScene *scene, ENTITY_ID entityId);
			void reset(::AScene *scene, ENTITY_ID entityId);

		protected:
			OctreeKey _cameraOTKey;

		};
	}
}