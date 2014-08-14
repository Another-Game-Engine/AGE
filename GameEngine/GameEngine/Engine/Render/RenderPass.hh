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
		~Render();

		Render() = delete;
		Render(Render const &copy) = delete;
		Render &operator=(Render const &r) = delete;

		virtual Render &update() = 0;

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
		
		// shader attach to this render
		Render &bindShader(Shader *shader);
		Shader *accessShader() const;

		//  drawing mode
		Render &setMode(GLenum mode);
		GLenum getMode() const;

		// Render attach to this render
		Render &bindInput(Render const &input);
		Render &unbindInput();

		// color ouput
		Render &addColorOutput(GLenum target, GLenum internalFormat);
		Texture2D const &getColorOutput(size_t index) const;
		size_t getNbrColorOuput() const;

	protected:
		AGE::Vector<Task> _tasks;
		AGE::Vector<std::pair<GLenum, Texture2D *>> _colorOutput;
		GLint _stencilSize;
		glm::ivec4 _rect;
		Shader *_shader;
		GLenum _mode;
		
		bool _updateColorOutput;

		Render const *_input;
		bool _updateInput;

		void updateInput();
		virtual void updateOutput() = 0;
	};

	class RenderPass : public Render
	{
	public:
		RenderPass();
		virtual ~RenderPass();
		RenderPass(RenderPass const &copy);
		RenderPass &operator=(RenderPass const &r);

		virtual Render &update();

		RenderPass &configSample(GLint sample);
	
	private:
		Framebuffer _fbo;
		GLint _sample;

		virtual void updateOutput();

	};

	class RenderPostEffect : public Render
	{
	public:
		RenderPostEffect();
		virtual ~RenderPostEffect();
		RenderPostEffect(RenderPostEffect const &copy);
		RenderPostEffect &operator=(RenderPostEffect const &r);

		virtual Render &update();

	private:
		gl::Key<Vertices> _quad;
		gl::Key<Indices> _indicesQuad;
		gl::Key<VertexPool> _quadPool;
		gl::Key<IndexPool> _indicesQuadPool;

		virtual void updateOutput();
	};

	__forceinline void Render::updateOutput()
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