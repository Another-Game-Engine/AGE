#include <Render/ShaderResource.hh>

static gl::ShaderResource shadersResources[gl::nbrShaderResource] = 
{
	{ sizeof(glm::mat3), GL_FLOAT_MAT3 },
	{ sizeof(glm::mat4), GL_FLOAT_MAT4 },
	{ sizeof(glm::mat2), GL_FLOAT_MAT2 },
	{ sizeof(float), GL_FLOAT },
	{ sizeof(glm::vec2), GL_FLOAT_VEC2 },
	{ sizeof(glm::vec3), GL_FLOAT_VEC3 },
	{ sizeof(glm::vec4), GL_FLOAT_VEC4 },
	{ sizeof(int), GL_INT },
	{ sizeof(glm::ivec2), GL_INT_VEC2 },
	{ sizeof(glm::ivec3), GL_INT_VEC3 },
	{ sizeof(glm::ivec4), GL_INT_VEC4 },
	{ sizeof(bool), GL_BOOL },
	{ sizeof(glm::bvec2), GL_BOOL_VEC2 },
	{ sizeof(glm::bvec3), GL_BOOL_VEC3 },
	{ sizeof(glm::bvec4), GL_BOOL_VEC4 }
};

namespace gl
{
	size_t convertGLidToSize(GLint type)
	{
		for (size_t index = 0; index < nbrShaderResource; ++index)
			if (shadersResources[index].type == type)
				return (shadersResources[index].size);
		assert(0);
		return (0);
	}

}