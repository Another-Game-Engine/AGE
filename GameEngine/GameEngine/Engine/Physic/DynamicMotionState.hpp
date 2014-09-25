#pragma once

#include <LinearMath/btMotionState.h>

#include <Utils/MatrixConversion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Link.hpp>

class DynamicMotionState : public btMotionState
{
protected:
    AGE::Link *_link;
    btTransform mInitialPosition;
public:
    DynamicMotionState(AGE::Link *link)
		: _link(link)
    {
		mInitialPosition.setFromOpenGLMatrix(glm::value_ptr(link->getTransform()));
    }

    virtual ~DynamicMotionState()
    {
    }

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        worldTrans.setFromOpenGLMatrix(glm::value_ptr(_link->getTransform()));
    }

    virtual void setWorldTransform(const btTransform &worldTrans)
    {
		_link->setPosition(convertBulletVectorToGLM(worldTrans.getOrigin()));
		_link->setOrientation(convertBulletQuatToGLM(worldTrans.getRotation()));
		//auto scale = scaleFromMat4(_ageTransform);
		//worldTrans.getOpenGLMatrix(glm::value_ptr(_ageTransform));
		//_ageTransform = glm::scale(_ageTransform, scale);
    }
};