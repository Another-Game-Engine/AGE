#pragma once

#include "Utils/Frustum.hh"

namespace AGE
{
	class BFCFrustumCuller : public BFCPrivate::CullerBase
	{
	public:
		void prepareForCulling(const Frustum &frustum)
		{
			_frustum = frustum;
		}
		inline void cullItem(const BFCItem &item)
		{
			if (item.getDrawable() && _frustum.checkCollision(item.getPosition()))
			{
				_cullerArray.push(item);
			}
		}

	private:
		Frustum            _frustum;
	};
}