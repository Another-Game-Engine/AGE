#pragma once

#include <Entities/EntityTypedef.hpp>
#include <cstddef>
#include <Core/Octree.hpp>

class AScene;

namespace AGE
{
	struct OctreeElement
	{
	public:
		OctreeElement();
		virtual ~OctreeElement();
		OctreeElement(OctreeElement &&o);
		OctreeElement &operator=(OctreeElement &&o);

		virtual OctreeElement &initOctree(AScene *scene, ENTITY_ID entityId) = 0;
		virtual OctreeElement &resetOctree(AScene *scene, ENTITY_ID entityId) = 0;

	protected:
		OctreeKey _OTKey;
		AScene *_scene = nullptr;
	};
}