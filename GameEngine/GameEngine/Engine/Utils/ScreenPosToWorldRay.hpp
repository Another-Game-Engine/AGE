#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

void screenPosToWorldRay(
	int mouseX,
	int mouseY,
	int screenWidth,
	int screenHeight,
	const glm::mat4 &viewMatrix,
	const glm::mat4 &projectionMatrix,
	glm::vec3& out_origin,
	glm::vec3& out_direction);