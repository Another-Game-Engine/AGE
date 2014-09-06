#pragma once

#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <Render/Key.hh>
#include <Render/MemoryGPU.hh>
#include <map>
#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>
#include <Render/UniformBlock.hh>
#include <Render/Material.hh>
#include <cassert>
#include <Render/GeometryManager.hh>
#include <Render/MaterialManager.hh>
#include <Render/Render.hh>
#include <Render/Shader.hh>
#include <Core/Drawable.hh>
#include <Render/MacroRenderManager.hh>
#include <Utils/CommandQueueHolder.hpp>

namespace gl
{
	struct Uniform;
	struct Sampler;
	struct InterfaceBlock;
	class Texture;
	class UniformBuffer;
	struct BindingRenderPass;
	struct BindingShader;
	class Pipeline;

	namespace RenderManagerCmd
	{
		struct Stop{};
		struct BoolFunction : public TMQ::FutureData < bool >
		{
			std::function<bool()> function;
			BoolFunction(const std::function<bool()> &_function)
				: function(_function)
			{
			}
		};
		struct VoidFunction
		{
			std::function<void(void)> function;
			VoidFunction(const std::function<void(void)> &_function)
				: function(_function)
			{
			}
			VoidFunction(const VoidFunction &) = delete;
			VoidFunction &operator=(const VoidFunction&) = delete;
			~VoidFunction()
			{
			}
		};
	}

	class RenderManager : public Dependency<RenderManager>, public AGE::CommandQueueHolder
	{
	public:
		GeometryManager geometryManager;
		MaterialManager materialManager;

	public:
		RenderManager();
		~RenderManager();

		// shader handling
		RenderManager &createPreShaderQuad();
		Key<Shader> addComputeShader(std::string const &compute);
		Key<Shader> addShader(std::string const &vert, std::string const &frag);
		Key<Shader> addShader(std::string const &geometry, std::string const &vert, std::string const &frag);
		Key<Shader> getShader(size_t index) const;
		
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat4 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat3 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, float value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::vec4 const &value);
		Key<Uniform> getShaderUniform(Key<Shader> const &shader, size_t index);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3);
		
		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		Key<Sampler> getShaderSampler(Key<Shader> const &shader, size_t index);
		RenderManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Key<Texture> const &keytexture);
		
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock);
		Key<InterfaceBlock> getShaderInterfaceBlock(Key<Shader> const &shader, size_t index);

		Key<UniformBlock> addUniformBlock(size_t nbrElement, size_t *sizeElement);
		RenderManager &rmUniformBlock(Key<UniformBlock> &uniformBlock);
		Key<UniformBlock> getUniformBlock(size_t index) const;
		template <typename TYPE> RenderManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value);
		RenderManager &bindTransformationToShader(Key<Shader> const &keyShader, Key<Uniform> const &keyUniform);
		template <typename TYPE> RenderManager &bindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		RenderManager &unbindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);

		// Texture
		Key<Texture> addTexture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipmapping);
		RenderManager &uploadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		RenderManager &downloadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		RenderManager &setlevelTargetTexture(Key<Texture> const &key, uint8_t level);
		RenderManager &bindTexture(Key<Texture> const &key);
		RenderManager &unbindTexture(Key<Texture> const &key);
		RenderManager &configUploadTexture2D(Key<Texture> const &key, glm::ivec4 const &rect);
		RenderManager &parameterTexture(Key<Texture> const &key, GLenum pname, GLint param);
		RenderManager &rmTexture(Key<Texture> &key);
		Key<Texture> getTexture(size_t target) const;
		GLenum getTypeTexture(Key<Texture> const &key);


		// RenderPass
		Key<RenderPass> addRenderPass(Key<Shader> const &shader, glm::ivec4 const &rect);
		Key<RenderPass> getRenderPass(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderPass)
		RenderManager &configRenderPass(Key<RenderPass> const &renderPass, glm::ivec4 const &rect, GLenum mode = GL_TRIANGLES, GLint sample = 1);
		RenderManager &pushOutputColorRenderPass(Key<RenderPass> const &key, GLenum attachement, GLenum internalFormat);
		RenderManager &popOutputColorRenderPass(Key<RenderPass> const &key);
		RenderManager &pushInputColorRenderPass(Key<RenderPass> const &key, Key<Sampler> const &s);
		RenderManager &popInputColorRenderPass(Key<RenderPass> const &key);
		RenderManager &createDepthBufferRenderPass(Key<RenderPass> const &key);
		RenderManager &createStencilBufferRenderPass(Key<RenderPass> const &key);
		RenderManager &useInputDepthRenderPass(Key<RenderPass> const &key);
		RenderManager &unUseInputDepthRenderPass(Key<RenderPass> const &key);
		RenderManager &useInputStencilRenderPass(Key<RenderPass> const &key);
		RenderManager &unUseInputStencilRenderPass(Key<RenderPass> const &key);
		RenderManager &useInputColorRenderPass(Key<RenderPass> const &key, GLenum attachement);
		RenderManager &unUseInputColorRenderPass(Key<RenderPass> const &key, GLenum attachement);

		// RenderPostEffect
		Key<RenderPostEffect> addRenderPostEffect(Key<Shader> const &s, glm::ivec4 const &rect);
		Key<RenderPostEffect> getRenderPostEffect(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderPostEffect)
		RenderManager &configRenderPostEffect(Key<RenderPostEffect> const &renderPass, glm::ivec4 const &rect, GLenum mode = GL_TRIANGLES, GLint sample = 1);
		RenderManager &pushOutputColorRenderPostEffect(Key<RenderPostEffect> const &key, GLenum attachement, GLenum internalFormat);
		RenderManager &popOutputColorRenderPostEffect(Key<RenderPostEffect> const &key);
		RenderManager &pushInputColorRenderPostEffect(Key<RenderPostEffect> const &key, Key<Sampler> const &s);
		RenderManager &popInputColorRenderPostEffect(Key<RenderPostEffect> const &key);

		// RenderOnScreen
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect);
		Key<RenderOnScreen> getRenderOnScreen(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderOnScreen);
		RenderManager &configRenderOnScreen(Key<RenderOnScreen> const &renderOnScreen, glm::ivec4 const &rect, GLenum mode);

		RenderManager &branch(Key<RenderPass> const &from, Key<RenderPass> const &to);
		RenderManager &branch(Key<RenderPass> const &from, Key<RenderPostEffect> const &to);
		RenderManager &branch(Key<RenderPass> const &from, Key<RenderOnScreen> const &to);
		RenderManager &branch(Key<RenderPostEffect> const &from, Key<RenderOnScreen> const &to);
	
		// Pipeline
		Key<Pipeline> addPipeline();
		RenderManager &setPipeline(Key<Pipeline> const &p, uint8_t time, Key<RenderPass> const &r);
		RenderManager &setPipeline(Key<Pipeline> const &p, uint8_t time, Key<RenderPostEffect> const &r);
		RenderManager &setPipeline(Key<Pipeline> const &p, uint8_t time, Key<RenderOnScreen> const &r);
		Key<Pipeline> getPipeline(size_t target);
		RenderManager &updatePipeline(Key<Pipeline> const &p, AGE::Vector<AGE::Drawable> const &objectRender);

		// drawing
		RenderManager &drawPipelines();
		RenderManager &drawPipeline(Key<Pipeline> const &key, AGE::Vector<AGE::Drawable> const &objectRender);
		RenderManager &draw(Key<RenderOnScreen> const &key, Key<RenderPass> const &r, AGE::Vector<AGE::Drawable> const &objectRender);

		// CommandQueue
		virtual bool updateCommandQueue()
		{
			bool result = true;
			_commandQueue.getDispatcher()
				.handle<RenderManagerCmd::BoolFunction>([&](RenderManagerCmd::BoolFunction& msg)
			{
				msg.result.set_value(msg.function());
			})
				.handle<RenderManagerCmd::VoidFunction>([&](const RenderManagerCmd::VoidFunction& msg)
			{
				if (msg.function)
					msg.function();
				else
					std::cout << "Prout" << std::endl;
			})
				.handle<RenderManagerCmd::Stop>([&](RenderManagerCmd::Stop& msg)
			{
				result = false;
			});
			return result;
		}
	private:
		// all map
		std::map<Key<Shader>, Shader *> _shaders;
		Shader * _preShaderQuad;
		std::map<Key<UniformBlock>, UniformBlock> _uniformBlock;
		std::map<Key<Texture>, Texture *> _textures;
		std::map<Key<RenderPass>, RenderPass *> _renderPass;
		std::map<Key<RenderPostEffect>, RenderPostEffect *> _renderPostEffect;
		std::map<Key<RenderOnScreen>, RenderOnScreen *> _renderOnScreen;
		std::map<Key<Pipeline>, Pipeline> _pipelines;
		uint8_t _minTime;
		uint8_t _maxTime;

		// optimize search in map
		std::pair<Key<Shader>, Shader *> _optimizeShaderSearch;
		std::pair<Key<UniformBlock>, UniformBlock *> _optimizeUniformBlockSearch;
		std::pair<Key<Texture>, Texture *> _optimizeTextureSearch;
		std::pair<Key<RenderPass>, RenderPass *> _optimizeRenderPassSearch;
		std::pair<Key<RenderPostEffect>, RenderPostEffect *> _optimizeRenderPostEffectSearch;
		std::pair<Key<RenderOnScreen>, RenderOnScreen *> _optimizeRenderOnScreenSearch;
		std::pair<Key<Pipeline>, Pipeline *> _optimizePipelineSearch;

		// tool use in intern for search
		Shader *getShader(Key<Shader> const &key);
		UniformBlock *getUniformBlock(Key<UniformBlock> const &key);
		Texture *getTexture(Key<Texture> const &key);
		RenderPass *getRenderPass(Key<RenderPass> const &key);
		RenderPostEffect *getRenderPostEffect(Key<RenderPostEffect> const &key);
		RenderOnScreen *getRenderOnScreen(Key<RenderOnScreen> const &key);
		Pipeline *getPipeline(Key<Pipeline> const &key);

	};

	template <typename TYPE>
	RenderManager &RenderManager::bindMaterialToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->bindingMaterial<TYPE>(uniformKey);
		return (*this);
	}

	template <typename TYPE>
	RenderManager &RenderManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value)
	{
		UniformBlock *uniformBlock;

		if ((uniformBlock = getUniformBlock(key)) == NULL)
			return (*this);
		uniformBlock->set<TYPE>(index, value);
		return (*this);
	}

	template <typename TYPE1>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1);
	}

	template <typename TYPE1, typename TYPE2>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3); tab[3] = sizeof(TYPE4);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4, typename TYPE5>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3); tab[3] = sizeof(TYPE4); tab[4] = sizeof(TYPE5);
	}

	template <typename TYPE1, typename TYPE2, typename TYPE3, typename TYPE4, typename TYPE5, typename TYPE6>
	inline void set_tab_sizetype(size_t *tab)
	{
		tab[0] = sizeof(TYPE1); tab[1] = sizeof(TYPE2); tab[2] = sizeof(TYPE3); tab[3] = sizeof(TYPE4); tab[4] = sizeof(TYPE5); tab[5] = sizeof(TYPE6);
	}
}