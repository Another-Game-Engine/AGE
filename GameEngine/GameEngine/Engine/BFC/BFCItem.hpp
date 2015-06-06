#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	struct BFCCullableObject;
	class BFCBlockManagerFactory;

	class BFCItem
	{
	public:
		BFCItem();
		void setDrawable(BFCCullableObject *drawable);
		void setPosition(const glm::vec4 &position);
	private:
		BFCCullableObject *_drawable = nullptr;  // 64
		void        *_transformationInfos; //64
		glm::vec4   _position; // 128

		friend class BFCBlockManagerFactory;
	};
}