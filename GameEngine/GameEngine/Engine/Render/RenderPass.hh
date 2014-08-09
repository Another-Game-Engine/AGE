#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>
#include <utility>
#include <Utils/Containers/Vector.hpp>
#include <functional>
#include <Render/Material.hh>
#include <Render/UniformBlock.hh>
#include <Render/OpenGLTask.hh>
#include <Render/Framebuffer.hh>

namespace AGE { class Drawable; }

namespace gl
{
	class Shader;
	struct Uniform;
	struct Sampler;
	class GeometryManager;
	class Texture2D;

	//!\file RenderPass.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class RenderPass
	//!\brief Handle one of opengl render pass
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();
		RenderPass(RenderPass const &copy);
		RenderPass &operator=(RenderPass const &r);

		RenderPass &update();

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

		RenderPass &addColorOutput(GLenum target, GLenum internalFormat);
		RenderPass &attachInput(RenderPass const &input);
		RenderPass &dettachInput();

		bool stencilSizeValid();
		RenderPass &config(glm::ivec4 const &rect, GLint sample = 1);
		RenderPass &bindShader(Shader *shader);
		Shader *accessShader() const;
		RenderPass &setMode(GLenum mode);
		GLenum getMode() const;


	private:
		AGE::Vector<Task> _tasks;
		GLint _stencilSize;
		Framebuffer _fbo;
		glm::ivec4 _rect;
		GLint _sample;
		Shader *_shader;
		RenderPass const *_input;
		GLenum _mode;
		bool _updateColorOutput;
		bool _updateInput;
		AGE::Vector<std::pair<GLenum, Texture2D *>> _colorOutput;
	};

}