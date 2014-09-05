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
#include <map>
#include <utility>

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

	enum RenderType
	{
		RENDER_POST_EFFECT = 0,
		RENDER_PASS,
		RENDER_ON_SCREEN
	};

	class Render
	{
	public:
		virtual ~Render();

		virtual Render &draw() = 0;
		virtual RenderType getType() const = 0;

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
		
		Render &pushInputSampler(Key<Sampler> const &key, GLenum attachement);
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
		AGE::Vector<std::pair<Key<Sampler>, GLenum>> _inputSamplers;
		RenderOffScreen const *_branch;

		GeometryManager &_geometryManager;
		void updateInput();
	};

	class RenderOffScreen : public Render
	{
	public:
		virtual ~RenderOffScreen();

		RenderOffScreen &configSample(GLint sample);

		RenderOffScreen &pushTarget(GLenum attachement);
		RenderOffScreen &popTarget();
		
		RenderOffScreen &createBufferSamplable(GLenum attachement, int x, int y, GLenum internalFormat);
		RenderOffScreen &createBufferSamplable(GLenum attachement, GLenum internalFormat);
		Texture2D const *getBufferSamplable(GLenum attachement) const;
		RenderOffScreen &createBufferNotSamplable(GLenum attachement, int x, int y, GLenum internalFormat);
		RenderOffScreen &createBufferNotSamplable(GLenum attachement, GLenum internalFormat);
		RenderBuffer const *getBufferNotSamplable(GLenum attachement) const;
		
		RenderOffScreen &deleteBuffer(GLenum attachement);

		RenderOffScreen &useInputBuffer(GLenum attachement);

	protected:
		RenderOffScreen(Shader &shader, GeometryManager &g);
		RenderOffScreen(RenderOffScreen const &copy) = delete;
		RenderOffScreen &operator=(RenderOffScreen const &r) = delete;

		std::map<GLenum, std::pair<Storage const *, bool>> _buffer; // the bool is use to determinate if the storage is own by this class or the branched one.

		AGE::Vector<GLenum> _target;
		Framebuffer _fbo;
		GLint _sample;

		bool _updateBuffer;
		bool _updateFrameBuffer;

		void updateBuffer();
		void updateFrameBuffer();
		void updateOutput();
	};

	class RenderOnScreen : public Render
	{
	public:
		virtual ~RenderOnScreen();
		RenderOnScreen(Key<Vertices> const &key, Shader &shader, GeometryManager &g);

		virtual Render &draw();
		virtual RenderType getType() const;
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
		virtual RenderType getType() const;
	
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
		virtual RenderType getType() const;
	private:
		RenderPostEffect(RenderPostEffect const &copy) = delete;
		RenderPostEffect &operator=(RenderPostEffect const &r) = delete;

		Key<Vertices> _quad;
	};

}