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
	class MaterialManager;
	class Texture2D;
	class RenderOffScreen;
	class Vertices;

	//!\file RenderPass.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class RenderPass
	//!\brief Handle one of opengl render pass
	class Render
	{
	public:
		~Render();

		virtual Render &draw() = 0;

		// prepare draw
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
		Render &configRect(glm::ivec4 const &rect);
		Render &setMode(GLenum mode);
		GLenum getMode() const;

		// Render attach to this render
		Render &branchInput(RenderOffScreen const &input);
		Render &unBranchInput();
		Render &pushInputSampler(Key<Sampler> const &key);
		Render &popInputSampler();

	protected:
		Render(Shader &shader, GeometryManager &g);
		
		Render() = delete;
		Render(Render const &copy) = delete;
		Render &operator=(Render const &r) = delete;

		glm::ivec4 _rect;
		GLenum _mode;
		
		Shader &_shader;
		AGE::Vector<Task> _tasks;
		AGE::Vector<Key<Sampler>> _inputSamplers;
		RenderOffScreen const *_branch;

		GeometryManager &_geometryManager;
		void updateInput();
	};

	class RenderOffScreen : public Render
	{
	public:
		virtual ~RenderOffScreen();

		RenderOffScreen &configSample(GLint sample);
		RenderOffScreen &pushColorOutput(GLenum attachement, GLenum internalFormat);
		RenderOffScreen &pushColorOutput(GLenum attachement, size_t width, size_t height, GLenum internalFormat);
		RenderOffScreen &popColorOutput();
		Texture2D const &getColorOutput(size_t index) const;
		GLenum getAttachementOutput(size_t index);
		size_t getNbrAttachementOutput() const;

		RenderOffScreen &createDepthBuffer();
		RenderOffScreen &deleteDepthBuffer();
		RenderBuffer const *getDepthBuffer() const;

		RenderOffScreen &createStencilBuffer();
		RenderOffScreen &deleteStencilBuffer();
		RenderBuffer const *getStencilBuffer() const;

		RenderOffScreen &useInputDepth();
		RenderOffScreen &unUseInputDepth();
		RenderOffScreen &useInputStencil();
		RenderOffScreen &unUseInputStencil();


	protected:
		RenderOffScreen(Shader &shader, GeometryManager &g);
		RenderOffScreen(RenderOffScreen const &copy) = delete;
		RenderOffScreen &operator=(RenderOffScreen const &r) = delete;

		GLenum *_colorAttachement;
		Texture2D **_colorTexture2D;
		uint8_t _nbrColorAttachement;

		RenderBuffer *_depthBuffer;
		RenderBuffer *_stencilBuffer;
		bool _useInputDepth;
		bool _useInputStencil;

		Framebuffer _fbo;
		GLint _sample;
		bool _updateOutput;

		void updateOutput();
		void updateInput();
	};

	class RenderOnScreen : public Render
	{
	public:
		virtual ~RenderOnScreen();
		RenderOnScreen(Key<Vertices> const &key, Shader &shader, GeometryManager &g);

		virtual Render &draw();
	private:
		RenderOnScreen(RenderOnScreen const &copy) = delete;
		RenderOnScreen &operator=(RenderOnScreen const &r) = delete;

		Key<Vertices> _quad;
	};

	class RenderPass : public RenderOffScreen
	{
	public:
		RenderPass(Shader &shader, GeometryManager &g, MaterialManager &m);
		virtual ~RenderPass();

		RenderPass &setRenderPassObjects(AGE::Vector<AGE::Drawable> const &objects);
		virtual Render &draw();
	
	private:
		RenderPass(RenderPass const &copy) = delete;
		RenderPass &operator=(RenderPass const &r) = delete;
	
		AGE::Vector<AGE::Drawable> const *_objectsToRender;

		MaterialManager &_materialManager;
	};

	class RenderPostEffect : public RenderOffScreen
	{
	public:
		RenderPostEffect(Key<Vertices> const &key, Shader &s, GeometryManager &g);
		virtual ~RenderPostEffect();

		virtual Render &draw();
	private:
		RenderPostEffect(RenderPostEffect const &copy) = delete;
		RenderPostEffect &operator=(RenderPostEffect const &r) = delete;

		Key<Vertices> _quad;
	};

}