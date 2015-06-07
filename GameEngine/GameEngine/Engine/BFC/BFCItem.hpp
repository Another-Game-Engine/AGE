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
		inline BFCCullableObject *getDrawable() { return _drawable; }
		void setPosition(const glm::vec4 &position);
		inline glm::vec4 getPosition() { return _position; }
	private:
		BFCCullableObject *_drawable = nullptr;  // 64
		void        *_transformationInfos; //64
		glm::vec4   _position; // 128

		friend class BFCBlockManagerFactory;
	};
}