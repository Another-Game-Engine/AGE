#pragma once

#include <LinearMath/btMotionState.h>

#include <Utils/MatrixConversion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class DynamicMotionState : public btMotionState
{
protected:
    glm::mat4 &_ageTransform;
    btTransform mInitialPosition;
public:
    DynamicMotionState(glm::mat4 &transform)
		: _ageTransform(transform)
    {
		mInitialPosition.setFromOpenGLMatrix(glm::value_ptr(transform));
    }

    virtual ~DynamicMotionState()
    {
    }

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        worldTrans.setFromOpenGLMatrix(glm::value_ptr(_ageTransform));
    }

    virtual void setWorldTransform(const btTransform &worldTrans)
    {
		auto scale = scaleFromMat4(_ageTransform);
		worldTrans.getOpenGLMatrix(glm::value_ptr(_ageTransform));
		_ageTransform = glm::scale(_ageTransform, scale);
    }
};