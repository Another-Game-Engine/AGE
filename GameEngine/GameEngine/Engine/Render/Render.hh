#pragma once

#include <Utils/OpenGL.hh>
#include <glm/glm.hpp>
#include <stdint.h>
#include <utility>
#include <Utils/Containers/Vector.hpp>
#include <functional>
#include <Render/Material.hh>
#include <Render/UniformBlock.hh>
#include <Render/Task.hh>
#include <Render/Framebuffer.hh>
#include <Render/Shader.hh>
#include <Render/LocationStorage.hh>
#include <map>
#include <utility>

namespace AGE { struct Drawable;  }

#define DECL_INPUT struct Input{ Key<Sampler> sampler; GLenum attachement; OffScreenRender const &render; Input(Key<Sampler> const &sampler, GLenum attachement, OffScreenRender const &render); Input(Input const &copy); ~Input(); };

namespace gl
{
	struct Uniform;
	struct Sampler;
	class GeometryManager;
	class MaterialManager;
	class Texture2D;
	class RenderOffScreen;
	class Vertices;
	class Indices;
	class IndexPool;
	class VertexPool;

	enum RenderType
	{
		RENDER_POST_EFFECT = 0,
		RENDER_PASS,
		RENDER_ON_SCREEN,
		RENDER_EMPTY
	};

	struct Draw
	{
		GLenum mode;
		GeometryManager &geometryManager;
		Shader &shader;
		MaterialManager &materialManager;
		AGE::Vector<AGE::Drawable> const *toRender;
		size_t start;
		size_t end;
		Draw(GeometryManager &g, Shader &s, MaterialManager &m, GLenum mode);
		Draw(Draw const &copy);
		~Draw();
	};

	class OperationBuffer
	{
	public:
		virtual ~OperationBuffer();

		OperationBuffer &pushSetScissorTask(glm::ivec4 const &area);
		OperationBuffer &pushSetClearValueTask(glm::vec4 const &color, float depth, uint8_t stencil);
		OperationBuffer &pushSetColorMaskTask(GLuint index, glm::bvec4 const &color);
		OperationBuffer &pushSetDepthMaskTask(bool depth);
		OperationBuffer &pushSetStencilMaskTask(uint8_t front, uint8_t back);
		OperationBuffer &pushClearTask(bool color, bool depth, bool stencil);
		OperationBuffer &pushSetStencilFunctionFrontFaceTask(GLenum func, int ref, uint8_t mask);
		OperationBuffer &pushSetStencilOperationFrontFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		OperationBuffer &pushSetStencilFunctionBackFaceTask(GLenum func, int ref, uint8_t mask);
		OperationBuffer &pushSetStencilOperationBackFaceTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		OperationBuffer &pushSetStencilFunctionTask(GLenum func, int ref, uint8_t mask);
		OperationBuffer &pushSetStencilOperationTask(GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		OperationBuffer &pushSetBlendTask(int drawBuffer, bool state);
		OperationBuffer &pushSetBlendEquationTask(GLenum colorMode, GLenum alphaMode);
		OperationBuffer &pushSetBlendEquationTask(GLenum mode);
		OperationBuffer &pushSetBlendFuncTask(GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
		OperationBuffer &pushSetBlendFuncTask(GLenum src, GLenum dest);
		OperationBuffer &pushSetBlendConstantTask(glm::vec4 const &blendColor);
		OperationBuffer &pushSetTestTask(bool scissor, bool stencil, bool depth);
		OperationBuffer &pushOwnTask(std::function<void(LocationStorage &)> const &func);
		OperationBuffer &popTask();

	protected:
		OperationBuffer(LocationStorage &locationStorage);
		OperationBuffer(OperationBuffer const &copy) = delete;
		OperationBuffer &operator=(OperationBuffer const &o) = delete;

		LocationStorage &_locationStorage;
		AGE::Vector<std::function<void(LocationStorage &)> *> _ownFunction;
		AGE::Vector<Task> _tasks;

		void update();
	};

	class BaseRender
	{
	public:
		BaseRender &configRect(glm::ivec4 const &rect);

		virtual BaseRender &render() = 0;
		virtual RenderType getType() const = 0;

	protected:
		BaseRender();
		~BaseRender();

		glm::ivec4 _rect;
		void update();
	};

	class QuadRender
	{
	public:
		virtual ~QuadRender();
	protected:
		QuadRender(Key<Vertices> const &key, Key<Indices> const &id, Key<VertexPool> const &vertexPool, Key<IndexPool> const &indexPool);
		QuadRender(QuadRender const &copy) = delete;
		QuadRender &operator=(QuadRender const &q) = delete;

		Key<Vertices> _vertices;
		Key<Indices> _id;
		Key<VertexPool> _vertexPool;
		Key<IndexPool> _indexPool;
	};

	class OffScreenRender : public OperationBuffer, public BaseRender
	{
	public:
		~OffScreenRender();
		OffScreenRender &configSample(GLint sample);
		OffScreenRender &pushTarget(GLenum attachement);
		OffScreenRender &popTarget();
		OffScreenRender &createBufferSamplable(GLenum attachement, int x, int y, GLenum internalFormat);
		OffScreenRender &createBufferSamplable(GLenum attachement, GLenum internalFormat);
		Texture2D const *getBufferSamplable(GLenum attachement) const;
		OffScreenRender &createBufferNotSamplable(GLenum attachement, int x, int y, GLenum internalFormat);
		OffScreenRender &createBufferNotSamplable(GLenum attachement, GLenum internalFormat);
		RenderBuffer const *getBufferNotSamplable(GLenum attachement) const;
		OffScreenRender &deleteBuffer(GLenum attachement);
		OffScreenRender &useInputBuffer(GLenum attachement, OffScreenRender const &render);

	protected:
		OffScreenRender(LocationStorage &locationStorage);

		std::map<GLenum, std::pair<Storage const *, bool>> _buffer; // the bool is use to determinate if the storage is own by this class or the branched one.
		AGE::Vector<GLenum> _target;
		Framebuffer _fbo;
		GLint _sample;
		bool _updateTarget;
		bool _updateFrameBuffer;

		void update();
	};

	class DrawableRender
	{
	public:
		virtual ~DrawableRender();

		DECL_INPUT
		DrawableRender &setMode(GLenum mode);
		GLenum getMode() const;
		DrawableRender &pushInputSampler(Key<Sampler> const &key, GLenum attachement, OffScreenRender const &render);
		DrawableRender &popInputSampler();

	protected:
		DrawableRender(Shader &shader, GeometryManager &geo);
		DrawableRender(DrawableRender const &copy) = delete;
		DrawableRender &operator=(DrawableRender const &d) = delete;
		void update();

		AGE::Vector<Input> _inputSamplers;
		GeometryManager &_geometryManager;
		Shader &_shader;
		GLenum _mode;
	};

	class RenderOnScreen : public DrawableRender, public OperationBuffer, public QuadRender, public BaseRender
	{
	public:
		virtual ~RenderOnScreen();
		RenderOnScreen(Shader &s, GeometryManager &g, LocationStorage &l);

		virtual BaseRender &render();
		virtual RenderType getType() const;

	private:
		RenderOnScreen(RenderOnScreen const &copy) = delete;
		RenderOnScreen &operator=(RenderOnScreen const &r) = delete;
	};

	class RenderPass : public DrawableRender, public OffScreenRender
	{
	public:
		RenderPass(Shader &shader, GeometryManager &g, MaterialManager &m, LocationStorage &l);
		virtual ~RenderPass();

		RenderPass &pushPassTask();
		RenderPass &setPass(AGE::Vector<AGE::Drawable> const &objects, size_t start, size_t end);
		RenderPass &setPass();
		
		virtual BaseRender &render();
		virtual RenderType getType() const;

	private:
		RenderPass(RenderPass const &copy) = delete;
		RenderPass &operator=(RenderPass const &r) = delete;

		MaterialManager &_materialManager;
		AGE::Vector<AGE::Drawable> const *_toRender;
		size_t _start;
		size_t _end;
	};

	class RenderPostEffect : public DrawableRender, public OffScreenRender, public QuadRender
	{
	public:
		RenderPostEffect(Shader &s, GeometryManager &g, LocationStorage &l);
		virtual ~RenderPostEffect();

		virtual BaseRender &render();
		virtual RenderType getType() const;

	private:
		RenderPostEffect(RenderPostEffect const &copy) = delete;
		RenderPostEffect &operator=(RenderPostEffect const &r) = delete;
	};

	class EmptyRenderPass : public OffScreenRender
	{
	public:
		EmptyRenderPass(LocationStorage &locationStorage);
		virtual ~EmptyRenderPass();

		virtual BaseRender &render();
		virtual RenderType getType() const;

	private:
		EmptyRenderPass(EmptyRenderPass const &copy) = delete;
		EmptyRenderPass &operator=(EmptyRenderPass const &r) = delete;
	};

}