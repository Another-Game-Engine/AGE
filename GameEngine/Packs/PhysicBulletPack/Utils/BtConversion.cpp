#include <Utils/BtConversion.hpp>

glm::mat4 convertBulletTransformToGLM(const btTransform& transform)
{
	float data[16];
	transform.getOpenGLMatrix(data);
	return glm::make_mat4(data);
}
btTransform convertGLMTransformToBullet(const glm::mat4 &transform)
{
	const float* data = glm::value_ptr(transform);
	btTransform bulletTransform;
	bulletTransform.setFromOpenGLMatrix(data);
	return bulletTransform;
}
btVector3 convertGLMVectorToBullet(const glm::vec3 &vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}
glm::vec3 convertBulletVectorToGLM(const btVector3& vector)
{
	return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
}