#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>

namespace gl
{

	static const uint8_t nbrShaderResource = 15;

	struct ShaderResource
	{
		size_t size;
		GLint type;
	};

	size_t convertGLidToSize(GLint type);
}