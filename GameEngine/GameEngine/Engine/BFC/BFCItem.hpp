#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	struct BFCCullableObject;

	class BFCItem
	{
	public:
		BFCItem();
		void setDrawable(BFCCullableObject *drawable);
	private:
		BFCCullableObject *_drawable = nullptr;  // 64
		void        *_transformationInfos; //64
		glm::vec4   _position; // 128
	};
}