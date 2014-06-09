#ifndef  __MATRIX_CONVERSION_HPP__
# define __MATRIX_CONVERSION_HPP__

//# define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

glm::vec3 posFromMat4(const glm::mat4 &mat);
glm::vec3 scaleFromMat4(const glm::mat4 &m);
glm::vec3 rotFromMat4(const glm::mat4 &m, bool toEuler);

#endif //__MATRIX_CONVERSION_HPP__