#pragma once

#include <glm/fwd.hpp>

void screenPosToWorldRay(
	int mouseX,
	int mouseY,
	int screenWidth,
	int screenHeight,
	const glm::mat4 &viewMatrix,
	const glm::mat4 &projectionMatrix,
	glm::vec3& out_origin,
	glm::vec3& out_direction);