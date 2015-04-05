#include "DynamicMotionState.hpp"
#include <Core/Link.hpp>
#include <Utils/MatrixConversion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Physics/Utils/BtConversion.hpp>

namespace AGE
{
	DynamicMotionState::DynamicMotionState(AGE::Link *link)
		: _link(link)
	{
		mInitialPosition.setFromOpenGLMatrix(glm::value_ptr(link->getGlobalTransform()));
	}

	DynamicMotionState::~DynamicMotionState()
	{
	}

	void DynamicMotionState::getWorldTransform(btTransform &worldTrans) const
	{
		worldTrans.setFromOpenGLMatrix(glm::value_ptr(_link->getGlobalTransform()));
	}

	void DynamicMotionState::setWorldTransform(const btTransform &worldTrans)
	{
		if (_link->isUserModified())
		{
			mInitialPosition.setFromOpenGLMatrix(glm::value_ptr(_link->getGlobalTransform()));
			_link->setUserModified(false);
			return;
		}
		_link->internalSetPosition(convertBulletVectorToGLM(worldTrans.getOrigin()));
		_link->internalSetOrientation(convertBulletQuatToGLM(worldTrans.getRotation()));
		//auto scale = scaleFromMat4(_ageTransform);
		//worldTrans.getOpenGLMatrix(glm::value_ptr(_ageTransform));
		//_ageTransform = glm::scale(_ageTransform, scale);
	}
}