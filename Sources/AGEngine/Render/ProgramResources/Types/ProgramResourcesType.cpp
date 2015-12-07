#include "ProgramResourcesType.hh"

std::unordered_map<GLenum, GlType> available_types = std::unordered_map<GLenum, GlType>
{
	std::make_pair(GL_FLOAT, GlType(GL_FLOAT, GL_FLOAT, sizeof(float), 1, std::string("float"))),
	std::make_pair(GL_FLOAT_VEC2, GlType(GL_FLOAT_VEC2, GL_FLOAT, sizeof(glm::vec2), 2, std::string("vec2"))),
	std::make_pair(GL_FLOAT_VEC3, GlType(GL_FLOAT_VEC3, GL_FLOAT, sizeof(glm::vec3), 3, std::string("vec3"))),
	std::make_pair(GL_FLOAT_VEC4, GlType(GL_FLOAT_VEC4, GL_FLOAT, sizeof(glm::vec4), 4, std::string("vec4"))),
	std::make_pair(GL_FLOAT_MAT4, GlType(GL_FLOAT_MAT4, GL_FLOAT, sizeof(glm::mat4), 16, std::string("mat4"))),
};

const std::array<GLenum, nbr_resources> available_resources = std::array<GLenum, nbr_resources>
{
	GL_UNIFORM,
	GL_UNIFORM_BLOCK,
	GL_ATOMIC_COUNTER_BUFFER,
	GL_PROGRAM_INPUT,
	GL_PROGRAM_OUTPUT,
	GL_TRANSFORM_FEEDBACK_VARYING,
	GL_TRANSFORM_FEEDBACK_BUFFER,
	GL_BUFFER_VARIABLE,
	GL_SHADER_STORAGE_BLOCK,
	GL_VERTEX_SUBROUTINE,
	GL_GEOMETRY_SUBROUTINE,
	GL_FRAGMENT_SUBROUTINE,
	GL_TESS_CONTROL_SUBROUTINE,
	GL_TESS_EVALUATION_SUBROUTINE,
	GL_COMPUTE_SUBROUTINE,
	GL_VERTEX_SUBROUTINE_UNIFORM,
	GL_GEOMETRY_SUBROUTINE_UNIFORM,
	GL_FRAGMENT_SUBROUTINE_UNIFORM,
	GL_TESS_CONTROL_SUBROUTINE_UNIFORM,
	GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
	GL_COMPUTE_SUBROUTINE_UNIFORM,
};