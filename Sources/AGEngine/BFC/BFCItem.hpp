#pragma once

#include <glm/glm.hpp>

namespace AGE
{
	struct BFCCullableObject;
	class BFCBlockManagerFactory;

	__declspec(align(16))
	class BFCItem
	{
	public:
		BFCItem();
		void setDrawable(BFCCullableObject *drawable);
		inline BFCCullableObject *getDrawable() { return _drawable; }
		inline const BFCCullableObject *getDrawable() const { return _drawable; }
		void setPosition(const glm::vec4 &position);
		inline glm::vec4 getPosition() { return _position; }
	private:
		BFCCullableObject *_drawable = nullptr;  // 64
		glm::vec4   _position; // 128
	public:
		void        *next; //64

		friend class BFCBlockManagerFactory;
	};
}