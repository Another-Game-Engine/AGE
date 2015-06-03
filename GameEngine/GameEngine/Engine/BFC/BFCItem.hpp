#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	struct BFCRootType;

	class BFCItem
	{
	public:
		BFCItem();
		void setDrawable(BFCRootType *drawable);
	private:
		BFCRootType *_drawable = nullptr;  // 64
		void        *_transformationInfos; //64
		glm::vec4   _position; // 128
	};
}