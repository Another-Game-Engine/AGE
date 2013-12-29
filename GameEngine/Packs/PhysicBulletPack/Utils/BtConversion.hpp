#ifndef  __BT_CONVERSION_HPP__
# define __BT_CONVERSION_HPP__

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 convertBulletTransformToGLM(const btTransform& transform);
btTransform convertGLMTransformToBullet(const glm::mat4 &transform);
btVector3 convertGLMVectorToBullet(const glm::vec3 &vector);
glm::vec3 convertBulletVectorToGLM(const btVector3& vector);

#endif   //__BT_CONVERSION_HPP__