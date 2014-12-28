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

static const size_t nbr_type = 3;
static std::unordered_map<GLenum, std::pair<size_t, std::string>> types =
{
	std::make_pair(GL_FLOAT, std::make_pair(sizeof(float), "float")),
	std::make_pair(GL_FLOAT_VEC4, std::make_pair(sizeof(glm::vec4), "vec4")),
	std::make_pair(GL_FLOAT_MAT4, std::make_pair(sizeof(glm::mat4), "mat4")),
};