#include <Utils/OpenGL.hh>
#include <Render/OpenGLTask/Tasks.hh>

namespace OpenGLTasks
{
	void set_clear_stencil(int value)
	{
		glClearStencil(value);
	}

	void set_clear_depth(float value)
	{
		glClearDepth(value);
	}

	void set_clear_color(glm::vec4 const &color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void clear_buffer(bool color, bool depth, bool stencil)
	{
		GLenum mode = (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0) | (stencil ? GL_STENCIL_BUFFER_BIT : 0);
		glClear(mode);
	}

	void set_depth_test(bool is)
	{
		is ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void set_scissor_test(bool is)
	{
		is ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
	}

	void set_stencil_test(bool is)
	{
		is ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	}

	void set_blend_test(bool is, int attach_output)
	{
		is ? glEnablei(GL_BLEND, attach_output) : glEnablei(GL_BLEND, attach_output);
	}
}