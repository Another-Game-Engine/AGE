#include <OpenGL/RenderPass.hh>
#include <cassert>
#include <string>
#include <OpenGL/Shader.hh>
#include <OpenGL/Drawable.hh>
#include <OpenGL/BufferSettings.hh>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }


namespace gl
{
	RenderPass::RenderPass()
		: _shader(NULL)
	{
	}

	RenderPass::RenderPass(Shader &shader)
		: RenderPass()
	{
		_shader = &shader;
	}

	RenderPass::~RenderPass()
	{
	}

	RenderPass::RenderPass(RenderPass const &copy)
		: _shader(copy._shader)
	{
	}

	RenderPass &RenderPass::operator=(RenderPass const &r)
	{
		if (this != &r)
		{
			_shader = r._shader;
		}
		return (*this);
	}

	RenderPass &RenderPass::pushSetScissorTask(glm::ivec4 const &area)
	{
		Task task;

		task.nbrParams = 1;
		task.params = new void *[task.nbrParams];
		task.params[0] = new glm::ivec4;
		memcpy(task.params[0], &area, sizeof(glm::ivec4));
		task.func = setScissor;
		_tasks.push_back(task);
		return (*this);
	}

	RenderPass &RenderPass::draw(Drawable *objectRender, size_t nbrObjectRender)
	{
		
		if (!_shader)
			DEBUG_MESSAGE("Warning", "RenderPass - use", "no shader assign on this renderPass", *this);
		_shader->use();
		return (*this);
	}

	
}