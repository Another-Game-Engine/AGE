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
#include <Render/Shader.hh>

namespace AGE { struct Drawable; }

namespace gl
{
	struct Uniform;
	struct Sampler;
	class GeometryManager;
	class Texture2D;

	//!\file RenderPass.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class RenderPass
	//!\brief Handle one of opengl render pass
	class Render
	{
	public:
		Render();
		~Render();
		Render(Render const &copy);
		Render &operator=(Render const &r);

		virtual Render &update();

		Render &pushSetScissorTask(glm::ivec4 const &area);
		Render &pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil);
		Render &pushSetColorMaskTask(GLuint index, glm::bvec4 const &color);
		Render &pushSetDepthMaskTask(bool depth);
		Render &pushSetStencilMaskTask(uint8_t front, uint8_t back);
		Render &pushClearTask(bool color, bool depth, bool stencil);
		Render &pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask);
		Render &pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		Render &pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask);
		Render &pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		Render &pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask);
		Render &pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		Render &pushSetBlendTask(int drawBuffer, bool state);
		Render &pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode);
		Render &pushSetBlendEquationTask(GLenum mode);
		Render &pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
		Render &pushSetBlendFuncTask(GLenum src, GLenum dest);
		Render &pushSetBlendConstantTask(glm::vec4 const &blendColor);
		Render &pushSetTestTask(bool scissor, bool stencil, bool depth);
		Render &popTask();

		bool stencilSizeValid();
		Render &configRect(glm::ivec4 const &rect);
		Render &bindShader(Shader *shader);
		Shader *accessShader() const;
		Render &setMode(GLenum mode);
		GLenum getMode() const;

		Render &bindInput(RenderPass const &input);
		Render &unbindInput();

	protected:
		AGE::Vector<Task> _tasks;
		GLint _stencilSize;
		glm::ivec4 _rect;
		Shader *_shader;
		GLenum _mode;

		RenderPass const *_input;
		bool _updateInput;

		void updateInput();

	};

	class RenderPass : public Render
	{
	public:
		RenderPass();
		~RenderPass();
		RenderPass(RenderPass const &copy);
		RenderPass &operator=(RenderPass const &r);

		virtual Render &update();

		RenderPass &configSample(GLint sample);
		RenderPass &addColorOutput(GLenum target, GLenum internalFormat);
		Texture2D const &getColorOutput(size_t index) const;
		size_t getNbrColorOuput() const;
	
	private:
		Framebuffer _fbo;
		GLint _sample;
		bool _updateColorOutput;
		AGE::Vector<std::pair<GLenum, Texture2D *>> _colorOutput;

		void updateOutput();
	};

	__forceinline void RenderPass::updateOutput()
	{
		// update output
		if (_updateColorOutput)
		{
			GLenum *targeted;
			targeted = new GLenum[_colorOutput.size()];
			for (size_t index = 0; index < _colorOutput.size(); ++index)
			{
				targeted[index] = _colorOutput[index].first;
				_colorOutput[index].second->attachement(_fbo, targeted[index]);
			}
			glDrawBuffers(GLsizei(_colorOutput.size()), targeted);
			_updateColorOutput = true;
		}
	}

	__forceinline void Render::updateInput()
	{
		// update input
		if (_updateInput && _input != NULL)
		{
			if (_input->getNbrColorOuput() != _shader->getNbrInputSampler())
				DEBUG_MESSAGE("Warning", "RenderPass - update", "attach invalid", );
			for (size_t index = 0; index < _input->getNbrColorOuput(); ++index)
			{
				Key<InputSampler> const &key = _shader->getInputSampler(index);
				_shader->setInputSampler(key, _input->getColorOutput(index));
			}
		}
	}
}