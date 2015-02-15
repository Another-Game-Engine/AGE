#pragma once

# include <glm/glm.hpp>

namespace OpenGLTasks
{
	void set_clear_stencil(int value);
	void set_clear_depth(float value);
	void set_clear_color(glm::vec4 const &color);
	void clear_buffer(bool color = true, bool depth = true, bool stencil = true);
	void set_depth_test(bool is);
	void set_scissor_test(bool is);
	void set_stencil_test(bool is);
	void set_blend(bool is, int attach_ouput);
	void set_blend_func(GLenum source_factor, GLenum destination_factor);
	void set_blend_equation(GLenum mode);
	void set_blend_func_separate(GLenum source_factor, GLenum destination_factor,
		GLenum source_factor_alpha, GLenum destination_factor_alpha);
}