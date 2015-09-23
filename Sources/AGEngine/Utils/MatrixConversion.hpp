#ifndef  __MATRIX_CONVERSION_HPP__
# define __MATRIX_CONVERSION_HPP__

//# define GLM_FORCE_RADIANS

#include <glm/fwd.hpp>

glm::vec3 posFromMat4(const glm::mat4 &mat);
glm::vec3 scaleFromMat4(const glm::mat4 &m);
glm::vec3 rotFromMat4(const glm::mat4 &m, bool toEuler);

#endif //__MATRIX_CONVERSION_HPP__