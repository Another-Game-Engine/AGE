#pragma once

#include <LinearMath/btMotionState.h>

namespace AGE
{
	struct Link;
	class DynamicMotionState : public btMotionState
	{
	protected:
		AGE::Link *_link;
		btTransform mInitialPosition;
	public:
		DynamicMotionState(AGE::Link *link);
		virtual ~DynamicMotionState();
		virtual void getWorldTransform(btTransform &worldTrans) const;
		virtual void setWorldTransform(const btTransform &worldTrans);
	};
}