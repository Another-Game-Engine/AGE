#include "BFCItem.hpp"

namespace AGE
{
	BFCItem::BFCItem()
		: _drawable(nullptr)
		, _transformationInfos(nullptr)
		, _position(glm::vec4(0,0,0,0))
	{

	}

	void BFCItem::setDrawable(BFCCullableObject *drawable)
	{
		_drawable = drawable;
	}

	void BFCItem::setPosition(const glm::vec4 &position)
	{
		_position = position;
	}

}