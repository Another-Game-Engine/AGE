#pragma once

#include <Entities/EntityTypedef.hpp>
#include <cstddef>
#include <Core/PrepareRenderThread.hpp>

class AScene;

namespace AGE
{
	struct PrepareElement
	{
	public:
		PrepareElement();
		virtual ~PrepareElement();
		PrepareElement(PrepareElement &&o);
		PrepareElement &operator=(PrepareElement &&o);

		virtual PrepareElement &initOctree(AScene *scene, ENTITY_ID entityId) = 0;
		virtual PrepareElement &resetOctree(AScene *scene, ENTITY_ID entityId) = 0;

	protected:
		PrepareKey _OTKey;
		AScene *_scene = nullptr;
	};
}