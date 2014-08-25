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
#include <Render/MacroShadingManager.hh>

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

	class ShadingManager : public Dependency<ShadingManager>
	{
	public:
		GeometryManager geometryManager;
		MaterialManager materialManager;

	public:
		ShadingManager();
		~ShadingManager();

		// shader handling
		ShadingManager &createPreShaderQuad();
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
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3);
		
		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		Key<Sampler> getShaderSampler(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Key<Texture> const &keytexture);
		
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock);
		Key<InterfaceBlock> getShaderInterfaceBlock(Key<Shader> const &shader, size_t index);

		Key<UniformBlock> addUniformBlock(size_t nbrElement, size_t *sizeElement);
		ShadingManager &rmUniformBlock(Key<UniformBlock> &uniformBlock);
		Key<UniformBlock> getUniformBlock(size_t index) const;
		template <typename TYPE> ShadingManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value);
		ShadingManager &bindTransformationToShader(Key<Shader> const &keyShader, Key<Uniform> const &keyUniform);
		template <typename TYPE> ShadingManager &bindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		ShadingManager &unbindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);

		// Texture
		Key<Texture> addTexture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipmapping);
		ShadingManager &uploadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		ShadingManager &downloadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		ShadingManager &setlevelTargetTexture(Key<Texture> const &key, uint8_t level);
		ShadingManager &bindTexture(Key<Texture> const &key);
		ShadingManager &unbindTexture(Key<Texture> const &key);
		ShadingManager &configUploadTexture2D(Key<Texture> const &key, glm::ivec4 const &rect);
		ShadingManager &parameterTexture(Key<Texture> const &key, GLenum pname, GLint param);
		ShadingManager &rmTexture(Key<Texture> &key);
		Key<Texture> getTexture(size_t target) const;
		GLenum getTypeTexture(Key<Texture> const &key);


		// RenderPass
		Key<RenderPass> addRenderPass(Key<Shader> const &shader, glm::ivec4 const &rect);
		Key<RenderPass> getRenderPass(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderPass)
		ShadingManager &configRenderPass(Key<RenderPass> const &renderPass, glm::ivec4 const &rect, GLenum mode = GL_TRIANGLES, GLint sample = 1);
		ShadingManager &pushOutputColorRenderPass(Key<RenderPass> const &key, GLenum attachement, GLenum internalFormat);
		ShadingManager &popOutputColorRenderPass(Key<RenderPass> const &key);
		ShadingManager &pushInputColorRenderPass(Key<RenderPass> const &key, Key<Sampler> const &s);
		ShadingManager &popInputColorRenderPass(Key<RenderPass> const &key);
		ShadingManager &createDepthBufferRenderPass(Key<RenderPass> const &key);
		ShadingManager &createStencilBufferRenderPass(Key<RenderPass> const &key);
		ShadingManager &useInputDepthRenderPass(Key<RenderPass> const &key);
		ShadingManager &unUseInputDepthRenderPass(Key<RenderPass> const &key);
		ShadingManager &useInputStencilRenderPass(Key<RenderPass> const &key);
		ShadingManager &unUseInputStencilRenderPass(Key<RenderPass> const &key);
		ShadingManager &useInputColorRenderPass(Key<RenderPass> const &key, GLenum attachement);
		ShadingManager &unUseInputColorRenderPass(Key<RenderPass> const &key, GLenum attachement);

		// RenderPostEffect
		Key<RenderPostEffect> addRenderPostEffect(Key<Shader> const &s, glm::ivec4 const &rect);
		Key<RenderPostEffect> getRenderPostEffect(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderPostEffect)
		ShadingManager &configRenderPostEffect(Key<RenderPostEffect> const &renderPass, glm::ivec4 const &rect, GLenum mode = GL_TRIANGLES, GLint sample = 1);
		ShadingManager &pushOutputColorRenderPostEffect(Key<RenderPostEffect> const &key, GLenum attachement, GLenum internalFormat);
		ShadingManager &popOutputColorRenderPostEffect(Key<RenderPostEffect> const &key);
		ShadingManager &pushInputColorRenderPostEffect(Key<RenderPostEffect> const &key, Key<Sampler> const &s);
		ShadingManager &popInputColorRenderPostEffect(Key<RenderPostEffect> const &key);

		// RenderOnScreen
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect);
		Key<RenderOnScreen> getRenderOnScreen(size_t target) const;
		GEN_DEC_RENDER_PUSH_TASK(RenderOnScreen);
		ShadingManager &configRenderOnScreen(Key<RenderOnScreen> const &renderOnScreen, glm::ivec4 const &rect, GLenum mode);

		ShadingManager &branch(Key<RenderPass> const &from, Key<RenderPass> const &to);
		ShadingManager &branch(Key<RenderPass> const &from, Key<RenderPostEffect> const &to);
		ShadingManager &branch(Key<RenderPass> const &from, Key<RenderOnScreen> const &to);
		ShadingManager &branch(Key<RenderPostEffect> const &from, Key<RenderOnScreen> const &to);
	
		// Pipeline
		Key<Pipeline> addPipeline();
		ShadingManager &setPipeline(Key<Pipeline> const &p, uint8_t time, Key<RenderPass> const &r);
		ShadingManager &setPipeline(Key<Pipeline> const &p, uint8_t time, Key<RenderPostEffect> const &r);
		ShadingManager &setPipeline(Key<Pipeline> const &p, uint8_t time, Key<RenderOnScreen> const &r);
		Key<Pipeline> getPipeline(size_t target);
		ShadingManager &updatePipeline(Key<Pipeline> const &p, AGE::Vector<AGE::Drawable> const &objectRender);

		// drawing
		ShadingManager &drawPipelines();
		ShadingManager &drawPipeline(Key<Pipeline> const &key, AGE::Vector<AGE::Drawable> const &objectRender);
		ShadingManager &draw(Key<RenderOnScreen> const &key, Key<RenderPass> const &r, AGE::Vector<AGE::Drawable> const &objectRender);

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
	ShadingManager &ShadingManager::bindMaterialToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey)) == NULL)
			return (*this);
		shader->bindingMaterial<TYPE>(uniformKey);
		return (*this);
	}

	template <typename TYPE>
	ShadingManager &ShadingManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value)
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