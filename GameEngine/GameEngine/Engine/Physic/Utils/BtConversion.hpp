#ifndef  __BT_CONVERSION_HPP__
# define __BT_CONVERSION_HPP__

#include <glm/fwd.hpp>

class btVertor3;
class btTransform;
class btVector3;
class btQuaternion;

glm::mat4 convertBulletTransformToGLM(const btTransform& transform);
btTransform convertGLMTransformToBullet(const glm::mat4 &transform);
btVector3 convertGLMVectorToBullet(const glm::vec3 &vector);
glm::vec3 convertBulletVectorToGLM(const btVector3& vector);
glm::quat convertBulletQuatToGLM(const btQuaternion& quat);
btQuaternion convertGLMQuatToBullet(const glm::quat& quat);


#endif   //__BT_CONVERSION_HPP__