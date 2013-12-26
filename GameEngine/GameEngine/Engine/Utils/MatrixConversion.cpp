#include <Utils/MatrixConversion.hpp>

glm::vec3 posFromMat4(const glm::mat4 &mat)
{
	return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
}

glm::vec3 scaleFromMat4(const glm::mat4 &m)
{
	// Extract col vectors of the matrix
	glm::vec3 col1(m[0][0], m[0][1], m[0][2]);
	glm::vec3 col2(m[1][0], m[1][1], m[1][2]);
	glm::vec3 col3(m[2][0], m[2][1], m[2][2]);
	//Extract the scaling factors
	glm::vec3 scaling;
	scaling.x = glm::length(col1);
	scaling.y = glm::length(col2);
	scaling.z = glm::length(col3);
	return scaling;
}

glm::vec3 rotFromMat4(const glm::mat4 &m, bool toEuler)
{
	glm::vec3 scale = scaleFromMat4(m);
	glm::vec3 columns[3] = {
		glm::vec3(m[0][0], m[0][1], m[0][2]),
		glm::vec3(m[1][0], m[1][1], m[1][2]),
		glm::vec3(m[2][0], m[2][1], m[2][2])
	};
	if (scale.x)
		columns[0] /= scale.x;
	if (scale.y)
		columns[1] /= scale.y;
	if (scale.z)
		columns[2] /= scale.z;
	glm::mat3 mat(columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z);
	glm::quat quat(mat);
	glm::vec3 euler = glm::eulerAngles(quat);// ;
	if (toEuler)
		euler *= 3.14159f / 180.f;
	else
		euler *= -1;
	return euler;
}