#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	class DrawablePtr;

	class BFCItem
	{
	public:
		BFCItem();
	private:
		DrawablePtr *_drawable = nullptr;  // 64
		void        *_transformationInfos; //64
		glm::vec4   _position; // 128
	};
}