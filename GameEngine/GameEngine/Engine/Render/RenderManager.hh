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
#include <Core/PreparableObject.hh>
#include <Render/MacroRenderManager.hh>
#include <Render/LocationStorage.hh>
#include <tmq/message.hpp>

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
	enum DrawType;

	class RenderManager : public Dependency<RenderManager>
	{
	public:
		GeometryManager geometryManager;
		LocationStorage locationStorage;

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
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::vec3 const &value);
		Key<Uniform> getShaderUniform(Key<Shader> const &shader, size_t index);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec3 const &vec3);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4);

		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		Key<Sampler> getShaderSampler(Key<Shader> const &shader, size_t index);
		RenderManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Key<Texture> const &keytexture);
		
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> &keyUniformBlock);
		RenderManager &setShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u);
		Key<InterfaceBlock> getShaderInterfaceBlock(Key<Shader> const &shader, size_t index);

		// uniform block
		Key<UniformBlock> addUniformBlock();
		RenderManager &introspectionBlock(Key<Shader> const &s, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u);
		RenderManager &rmUniformBlock(Key<UniformBlock> &uniformBlock);
		Key<UniformBlock> getUniformBlock(size_t index) const;
		template <typename TYPE> RenderManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value);
		template <typename TYPE> RenderManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value, size_t indexTab);

		RenderManager &bindTransformationToShader(Key<Shader> const &keyShader, Key<Uniform> const &keyUniform);
		template <typename TYPE> RenderManager &bindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		template <typename TYPE> RenderManager &bindMaterialToShader(Key<Shader> const &s, Key<Sampler> const &u);
		RenderManager &unbindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);

		// Material
		Key<Material> getDefaultMaterial();
		Key<Material> addMaterial();
		RenderManager &rmMaterial(Key<Material> &key);
		Key<Material> getMaterial(size_t index);
		template <typename TYPE> RenderManager &setMaterial(Key<Material> const &key, typename TYPE::return_type const &value);
		template <typename TYPE> RenderManager &setMaterial(Key<Material> const &key, Key<Texture> const &key_tex);
		template <typename TYPE> typename TYPE::return_type getMaterial(Key<Material> const &key);


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
		RenderManager &pushDrawTaskRenderBuffer(Key<RenderPass> const &key);
		GEN_DEC_RENDER_PUSH_TASK(RenderPass);
		GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(RenderPass);
		GEN_DEC_DRAWABLERENDER_PUSH_TASK(RenderPass);

		// RenderPostEffect
		Key<RenderPostEffect> addRenderPostEffect(Key<Shader> const &s, glm::ivec4 const &rect);
		Key<RenderPostEffect> getRenderPostEffect(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderPostEffect);
		GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(RenderPostEffect);
		GEN_DEC_DRAWABLERENDER_PUSH_TASK(RenderPostEffect);

		// RenderOnScreen
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPass> const &renderPass);
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPostEffect> const &renderPostEffect);
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect, Key<EmptyRenderPass> const &renderPostEffect);
		Key<RenderOnScreen> getRenderOnScreen(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderOnScreen);
		RenderManager &configRenderOnScreen(Key<RenderOnScreen> const &renderOnScreen, glm::ivec4 const &rect, GLenum mode);

		// EmptyRender
		Key<EmptyRenderPass> addEmptyRenderPass(glm::ivec4 const &rect);
		Key<EmptyRenderPass> getEmptyRenderPass(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(EmptyRenderPass);
		GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(EmptyRenderPass);


		// Pipeline
		Key<Pipeline> addPipeline();
		RenderManager &pushRenderPassPipeline(Key<Pipeline> const &p, Key<RenderPass> const &r);
		RenderManager &pushRenderPostEffectPipeline(Key<Pipeline> const &p, Key<RenderPostEffect> const &r);
		RenderManager &pushRenderOnScreenPipeline(Key<Pipeline> const &p, Key<RenderOnScreen> const &r);
		RenderManager &pushEmptyRenderPassPipeline(Key<Pipeline> const &p, Key<EmptyRenderPass> const &r);
		Key<Pipeline> getPipeline(size_t target);
		RenderManager &configPipeline(Key<Pipeline> const &key, DrawType type);
		RenderManager &updatePipeline(Key<Pipeline> const &p, AGE::Vector<AGE::Drawable> const &objectRender);

		// drawing
		RenderManager &drawPipelines();
		RenderManager &drawPipeline(Key<Pipeline> const &key, AGE::Vector<AGE::Drawable> const &objectRender);
		RenderManager &draw(Key<RenderOnScreen> const &key, Key<RenderPass> const &r, AGE::Vector<AGE::Drawable> const &objectRender);
	private:
		// internal manager
		MaterialManager _materialManager;

		// container
		std::map<Key<Shader>, Shader *> _shaders;
		Shader * _preShaderQuad;
		std::map<Key<UniformBlock>, UniformBlock> _uniformBlock;
		std::map<Key<Texture>, Texture *> _textures;
		std::map<Key<Pipeline>, size_t> _pipelines;
		AGE::Vector<Pipeline> _pipelineOrdered;

		// optimize search in map
		std::pair<Key<Shader>, Shader *> _optimizeShaderSearch;
		std::pair<Key<UniformBlock>, UniformBlock *> _optimizeUniformBlockSearch;
		std::pair<Key<Texture>, Texture *> _optimizeTextureSearch;
		std::pair<Key<Pipeline>, size_t> _optimizePipelineSearch;

		GEN_CONTAINER(RenderPass, _renderPass);
		GEN_CONTAINER(RenderPostEffect, _renderPostEffect);
		GEN_CONTAINER(RenderOnScreen, _renderOnScreen);
		GEN_CONTAINER(EmptyRenderPass, _emptyRenderPass);

		// tool use in intern for search
		Shader *getShader(Key<Shader> const &key);
		UniformBlock *getUniformBlock(Key<UniformBlock> const &key);
		Texture *getTexture(Key<Texture> const &key);
		GEN_DEC_SEARCH_FUNCTION(EmptyRenderPass);
		GEN_DEC_SEARCH_FUNCTION(RenderPass);
		GEN_DEC_SEARCH_FUNCTION(RenderOnScreen);
		GEN_DEC_SEARCH_FUNCTION(RenderPostEffect);

		size_t getIndexPipeline(Key<Pipeline> const &key);
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
	RenderManager &RenderManager::bindMaterialToShader(Key<Shader> const &shaderKey, Key<Sampler> const &samplerKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->bindingMaterial<TYPE>(samplerKey);
		return (*this);
	}

	template <typename TYPE>
	RenderManager &RenderManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value)
	{
		UniformBlock *uniformBlock = getUniformBlock(key);
		uniformBlock->set<TYPE>(index, value);
		return (*this);
	}

	template <typename TYPE>
	RenderManager &RenderManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value, size_t indexTab)
	{
		UniformBlock *uniformBlock;

		if ((uniformBlock = getUniformBlock(key)) == NULL)
			return (*this);
		uniformBlock->set<TYPE>(index, value, indexTab);
		return (*this);
	}

	template <typename TYPE>
	RenderManager &RenderManager::setMaterial(Key<Material> const &key, typename TYPE::return_type const &value)
	{
		_materialManager.setMaterial<TYPE>(key, value);
		return (*this);
	}

	template <typename TYPE>
	RenderManager &RenderManager::setMaterial(Key<Material> const &key, Key<Texture> const &key_tex)
	{
		Texture *texture = getTexture(tex);
		_materialManager.setMaterial<TYPE>(key, texture->getId());
		return (*this);
	}

	template <typename TYPE>
	typename TYPE::return_type RenderManager::getMaterial(Key<Material> const &key)
	{
		return (_materialManager.getMaterial<TYPE>(key);)
	}
}