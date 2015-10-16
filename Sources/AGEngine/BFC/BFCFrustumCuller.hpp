#pragma once

#include "Utils/Frustum.hh"

namespace AGE
{
	class BFCFrustumCuller : public BFCCullerMethod<BFCFrustumCuller>
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
		BFCFrustumCuller &operator=(BFCFrustumCuller &o)
		{
			_frustum = o._frustum;
			return *this;
		}
	private:
		Frustum            _frustum;
	};
}