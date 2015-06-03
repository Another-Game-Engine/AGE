#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	class BFCItem
	{
	public:
		BFCItem();
		void setDrawable(void *drawable);
	private:
		void        *_drawable = nullptr;  // 64
		void        *_transformationInfos; //64
		glm::vec4   _position; // 128
	};
}