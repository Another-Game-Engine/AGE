#pragma once

#include <array>
#include <unordered_map>
#include <glm/glm.hpp>

static size_t const nbr_resources = 21;

static std::array<GLenum, nbr_resources> const available_resources =
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

struct GlType
{
	uint8_t nbr_component;
	GLenum type;
	GLenum type_component;
	size_t size;
	std::string name;
	GlType(){}
	GlType(GLenum type, GLenum typeComponent, size_t size, size_t nbrComponent, std::string &&name) :
		nbr_component(nbrComponent),
		type(type),
		type_component(typeComponent),
		size(size),
		name(std::move(name))
	{}
	GlType(GlType const &copy) :
		nbr_component(copy.nbr_component),
		type(copy.type),
		type_component(copy.type_component),
		size(copy.size),
		name(copy.name)
	{}
};

static const size_t nbr_type = 4;
static std::unordered_map<GLenum, GlType> available_types =
{
	std::make_pair(GL_FLOAT, GlType(GL_FLOAT, GL_FLOAT, sizeof(float), 1, std::string("float"))),
	std::make_pair(GL_FLOAT_VEC3, GlType(GL_FLOAT_VEC3, GL_FLOAT, sizeof(glm::vec3), 3, std::string("vec3"))),
	std::make_pair(GL_FLOAT_VEC4, GlType(GL_FLOAT_VEC4, GL_FLOAT, sizeof(glm::vec4), 4, std::string("vec4"))),
	std::make_pair(GL_FLOAT_MAT4, GlType(GL_FLOAT_MAT4, GL_FLOAT, sizeof(glm::mat4), 16, std::string("mat4"))),
};