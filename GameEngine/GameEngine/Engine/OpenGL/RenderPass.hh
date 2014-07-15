#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>
#include <tuple>
#include <utility>
#include <vector>
#include <functional>

namespace AGE { class Drawable; }

namespace gl
{
	class Shader;

	// set buffer config
	//void setClearValue(glm::vec4 const &color, float depth, uint8_t stencil);
	//void setColorMask(GLuint index, glm::bvec4 const &color);
	//void setDepthMask(bool depth);
	//void setStencilMask(uint8_t front, uint8_t back);
	//void clear(bool color, bool depth, bool stencil);
	//void setScissor(glm::ivec4 const &area);
	//void setStencilFunctionFrontFace(GLenum func, int ref, uint8_t mask);
	//void setStencilOperationFrontFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
	//void setStencilFunctionBackFace(GLenum func, int ref, uint8_t mask);
	//void setStencilOperationBackFace(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
	//void setStencilFunction(GLenum func, int ref, uint8_t mask);
	//void setStencilOperation(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
	//void setBlend(int drawBuffer, bool state);
	//void setBlendEquation(GLenum colorMode, GLenum alphaMode);
	//void setBlendEquation(GLenum mode);
	//void setBlendFunc(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
	//void setBlendFunc(GLenum src, GLenum dest);
	//void setBlendConstant(glm::vec4 const &blendColor);
	//void setTest(bool scissor, bool stencil, bool depth);

	struct Task
	{
		int nbrParams;
		void **params;
		void(*func)(void **);
		Task() : nbrParams(0), params(NULL), func(NULL){}
	};

	//!\file RenderPass.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class RenderPass
	//!\brief Handle one of opengl render pass
	class RenderPass
	{
	public:
		RenderPass();
		RenderPass(Shader &shader);
		~RenderPass();
		RenderPass(RenderPass const &copy);
		RenderPass &operator=(RenderPass const &r);

		RenderPass &draw(AGE::Drawable const *objectRender, size_t nbrObjectRender);
		
		RenderPass &pushSetScissorTask(glm::ivec4 const &area);
		RenderPass &pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil);
		RenderPass &pushSetColorMaskTask(GLuint index, glm::bvec4 const &color);
		RenderPass &pushSetDepthMaskTask(bool depth);
		RenderPass &pushSetStencilMaskTask(uint8_t front, uint8_t back);
		RenderPass &pushClearTask(bool color, bool depth, bool stencil);
		RenderPass &pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask);
		RenderPass &pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		RenderPass &pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask);
		RenderPass &pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		RenderPass &pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask);
		RenderPass &pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		RenderPass &pushSetBlendTask(int drawBuffer, bool state);
		RenderPass &pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode);
		RenderPass &pushSetBlendEquationTask(GLenum mode);
		RenderPass &pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
		RenderPass &pushSetBlendFuncTask(GLenum src, GLenum dest);
		RenderPass &pushSetBlendConstantTask(glm::vec4 const &blendColor);
		RenderPass &pushSetTestTask(bool scissor, bool stencil, bool depth);
		RenderPass &popTask();

	private:
		Shader *_shader;
		std::vector<Task> _tasks;

		// Tool use in intern
		template <typename TYPE> void setAllocation(Task &task, TYPE elements);
		template <typename TYPE, typename... TYPES> void setAllocation(Task &task, TYPE element, TYPES... elements);
	};

	template <typename TYPE>
	void RenderPass::setAllocation(Task &task, TYPE element)
	{
		int index = task.nbrParams;
		task.nbrParams = task.nbrParams + 1;
		task.params = new void *[task.nbrParams];
		task.params[index] = new TYPE;
		memcpy(task.params[index], &element, sizeof(TYPE));
	}

	template <typename TYPE, typename... TYPES>
	void RenderPass::setAllocation(Task &task, TYPE element, TYPES... elements)
	{
		int index = task.nbrParams;
		task.nbrParams = task.nbrParams + 1;
		setAllocation(task, elements...);
		task.params[index] = new TYPE;
		memcpy(task.params[index], &element, sizeof(TYPE));
	}

}