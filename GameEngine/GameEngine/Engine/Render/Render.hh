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

	enum RenderingObjectType
	{
		GLOBAL_RENDER = 0,
		SEPARATE_RENDER
	};

	class Render
	{
	public:
		struct Draw
		{
		public:
			GeometryManager &geometryManager;
			Shader &shader;
			GLenum mode;
		public:
			Draw(GeometryManager &g, Shader &s, GLenum mode);
		};

	public:
		virtual ~Render();

		virtual Render &render() = 0;
		virtual RenderType getType() const = 0;

		// state
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
		Render(Draw *draw);
		Render() = delete;
		Render(Render const &copy) = delete;
		Render &operator=(Render const &r) = delete;
		void updateInput();

		glm::ivec4 _rect;
		AGE::Vector<Task> _tasks;
		AGE::Vector<std::pair<Key<Sampler>, GLenum>> _inputSamplers;
		RenderOffScreen const *_branch;
		Draw &_draw;
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
		RenderOffScreen(Render::Draw *draw);
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
		struct Draw : public Render::Draw
		{
		public:
			Key<Vertices> quad;

		public:
			Draw(GeometryManager &g, Shader &s, GLenum mode, Key<Vertices> const &quad);
		};

	public:
		virtual ~RenderOnScreen();
		RenderOnScreen(Key<Vertices> const &key, Shader &shader, GeometryManager &g);

		virtual Render &render();
		virtual RenderType getType() const;

	private:
		RenderOnScreen(RenderOnScreen const &copy) = delete;
		RenderOnScreen &operator=(RenderOnScreen const &r) = delete;

		Draw &_draw;
	};

	class RenderPass : public RenderOffScreen
	{
	public:
		struct Draw : public Render::Draw
		{
		public:
			MaterialManager &materialManager;
			AGE::Vector<AGE::Drawable> const *toRender;
			size_t start;
			size_t end;

		public:
			Draw(GeometryManager &g, Shader &s, MaterialManager &m, GLenum mode);
		};

	public:
		RenderPass(Shader &shader, GeometryManager &g, MaterialManager &m);
		virtual ~RenderPass();

		RenderPass &pushDrawTask();

		RenderPass &setTypeOfRenderingObjects(RenderingObjectType type);
		RenderPass &setObjectsToRender(AGE::Vector<AGE::Drawable> const &objects);
		virtual Render &render();
		virtual RenderType getType() const;

	private:
		void separateDraw();
		void globalDraw();
		RenderPass(RenderPass const &copy) = delete;
		RenderPass &operator=(RenderPass const &r) = delete;
	
		RenderingObjectType _typeRendering;
		Draw &_draw;
	};

	class RenderPostEffect : public RenderOffScreen
	{
	public:
		struct Draw : public Render::Draw
		{
		public:
			Key<Vertices> quad;

		public:
			Draw(GeometryManager &g, Shader &s, GLenum mode, Key<Vertices> const &quad);
		};
	public:
		RenderPostEffect(Key<Vertices> const &key, Shader &s, GeometryManager &g);
		virtual ~RenderPostEffect();

		virtual Render &render();
		virtual RenderType getType() const;

	private:
		RenderPostEffect(RenderPostEffect const &copy) = delete;
		RenderPostEffect &operator=(RenderPostEffect const &r) = delete;

		Draw &_draw;
	};

}