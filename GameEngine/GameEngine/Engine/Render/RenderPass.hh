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
		Render(Shader &shader);
		
		Render() = delete;
		Render(Render const &copy) = delete;
		Render &operator=(Render const &r) = delete;

		glm::ivec4 _rect;
		GLenum _mode;
		
		Shader &_shader;
		AGE::Vector<Task> _tasks;
		AGE::Vector<Key<Sampler>> _inputSamplers;
		RenderOffScreen const *_branch;
		bool _updateInput;
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

	protected:
		RenderOffScreen(Shader &shader, GeometryManager &g);
		RenderOffScreen(RenderOffScreen const &copy) = delete;
		RenderOffScreen &operator=(RenderOffScreen const &r) = delete;

		GLenum *_colorAttachement;
		Texture2D **_colorTexture2D;
		GLsizei _nbrColorAttachement;

		GeometryManager &_geometryManager;

		Framebuffer _fbo;
		GLint _sample;
		bool _updateOutput;
		void updateOutput();
	};

	__forceinline void RenderOffScreen::updateOutput()
	{
		glDrawBuffers(_nbrColorAttachement, _colorAttachement);
		_updateOutput = false;
	}

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

	//class RenderPostEffect : public Render
	//{
	//public:
	//	RenderPostEffect();
	//	virtual ~RenderPostEffect();
	//	RenderPostEffect(RenderPostEffect const &copy);
	//	RenderPostEffect &operator=(RenderPostEffect const &r);
	//
	//	virtual Render &update();
	//
	//private:
	//	gl::Key<Vertices> _quad;
	//	gl::Key<Indices> _indicesQuad;
	//	gl::Key<VertexPool> _quadPool;
	//	gl::Key<IndexPool> _indicesQuadPool;
	//
	//	virtual void updateOutput();
	//};
	//
	//__forceinline void Render::updateInput()
	//{
	//	// update input
	//	if (_updateInput && _input != NULL)
	//	{
	//		if (_input->getNbrColorOuput() != _inputSamplers.size())
	//			DEBUG_MESSAGE("Warning", "RenderPass - update", "attach invalid", );
	//		for (size_t index = 0; index < _input->getNbrColorOuput(); ++index)
	//			_shader->setSampler(_inputSamplers[index], _input->getColorOutput(index));
	//	}
	//}
}