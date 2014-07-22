#pragma once

#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <OpenGL/Key.hh>
#include <OpenGL/MemoryGPU.hh>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <OpenGL/UniformBlock.hh>
#include <OpenGL/Material.hh>
#include <cassert>
#include <OpenGL/GeometryManager.hh>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }

namespace AGE { class Drawable; }

namespace gl
{
	class Shader;
	struct Uniform;
	struct Sampler;
	struct InterfaceBlock;
	class Texture;
	class UniformBuffer;
	class RenderPass;
	class Drawable;

	//!\file ShadingManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class ShadingManager
	//!\brief Handle the shading of the object
	class ShadingManager : public Dependency<ShadingManager>
	{
	public:
		GeometryManager geometryManager;

	public:
		ShadingManager();
		~ShadingManager();

		// shader handling
		Key<Shader> addShader(std::string const &compute);
		Key<Shader> addShader(std::string const &vert, std::string const &frag);
		Key<Shader> addShader(std::string const &geometry, std::string const &vert, std::string const &frag);
		ShadingManager &rmShader(Key<Shader> &shader);
		Key<Shader> getShader(size_t index) const;
		ShadingManager &updateMemoryShader(Key<Shader> const &shader);
		
		// uniform
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat4 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat3 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, float value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::vec4 const &value);
		ShadingManager &rmShaderUniform(Key<Shader> const &shader, Key<Uniform> &uniform);
		Key<Uniform> getShaderUniform(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v);
		ShadingManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3);
		
		// sampler
		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		ShadingManager &rmShaderSampler(Key<Shader> const &shader, Key<Sampler> &uniform);
		Key<Sampler> getShaderSampler(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Key<Texture> const &keytexture);
	
		// Interface
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock);
		ShadingManager &rmShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> &key);
		Key<InterfaceBlock> getShaderInterfaceBlock(Key<Shader> const &shader, size_t index);

		// uniform Block
		Key<UniformBlock> addUniformBlock(size_t nbrElement, size_t *sizeElement);
		ShadingManager &rmUniformBlock(Key<UniformBlock> &uniformBlock);
		Key<UniformBlock> getUniformBlock(size_t index) const;
		template <typename TYPE> ShadingManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value);

		// Material
		Key<Material> addMaterial();
		ShadingManager &rmMaterial(Key<Material> &key);
		Key<Material> getMaterial(size_t index) const;
		template <typename TYPE> ShadingManager &setMaterial(Key<Material> const &key, typename TYPE::return_type const &value);
		template <typename TYPE> typename TYPE::return_type getMaterial(Key<Material> const &key);


		// Texture
		Key<Texture> addTexture2D(GLenum internalFormat, GLsizei width, GLsizei height, bool mipmapping);
		Key<Texture> addTextureMultiSample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocation);
		ShadingManager &wrapTexture2D(Key<Texture> const &key, GLint param);
		ShadingManager &filterTexture2D(Key<Texture> const &key, GLint param);
		ShadingManager &filterTexture2D(Key<Texture> const &key, GLint minFilter, GLint magFilter);
		ShadingManager &storageTexture2D(Key<Texture> const &key, GLint param);
		ShadingManager &storageTexture2D(Key<Texture> const &key, GLint pack, GLint unpack) ;
		ShadingManager &setOptionTransferTexture2D(Key<Texture> const &key, GLint level = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
		ShadingManager &generateMipMapTexture2D(Key<Texture> const &key);
		std::uint8_t getMaxLevelMipMapTexture2D(Key<Texture> const &key);
		ShadingManager &writeTexture(Key<Texture> const &key, void *write);
		ShadingManager &readTexture(Key<Texture> const &key, void *read);
		ShadingManager &bindTexture(Key<Texture> const &key);
		ShadingManager &unbindTexture(Key<Texture> const &key);
		ShadingManager &rmTexture(Key<Texture> &key);
		Key<Texture> getTexture(size_t target) const;
		GLenum getTypeTexture(Key<Texture> const &key);

		// RenderPass
		Key<RenderPass> addRenderPass(Key<Shader> const &shader);
		ShadingManager &rmRenderPass(Key<RenderPass> &key);
		Key<RenderPass> getRenderPass(size_t target) const;
		
		ShadingManager &pushClearTaskRenderPass(Key<RenderPass> const &key, bool color = true, bool depth = true, bool stencil = false);
		ShadingManager &pushSetClearValueTaskRenderPass(Key<RenderPass> const &key, glm::vec4 const &color, float depth = 1.0f, uint8_t stencil = 0);
		ShadingManager &pushSetColorMaskTaskRenderPass(Key<RenderPass> const &key, glm::bvec4 const &color, GLuint index = 0);
		ShadingManager &pushSetDepthMaskTaskRenderPass(Key<RenderPass> const &key, bool depth);
		ShadingManager &pushSetStencilMaskTaskRenderPass(Key<RenderPass> const &key, uint8_t front, uint8_t back);
		ShadingManager &pushSetTestTaskRenderPass(Key<RenderPass> const &key, bool scissor, bool stencil, bool depth);
		ShadingManager &pushSetScissorTaskRenderPass(Key<RenderPass> const &key, glm::ivec4 const &area);
		ShadingManager &pushSetStencilFunctionFrontFaceTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		ShadingManager &pushSetStencilOperationFrontFaceTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		ShadingManager &pushSetStencilFunctionBackFaceTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		ShadingManager &pushSetStencilOperationBackFaceTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		ShadingManager &pushSetStencilFunctionTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		ShadingManager &pushSetStencilOperationTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		ShadingManager &pushSetBlendEquationTaskRenderPass(Key<RenderPass> const &key, GLenum mode);
		ShadingManager &pushSetBlendEquationTaskRenderPass(Key<RenderPass> const &key, GLenum colorMode, GLenum alphaMode);
		ShadingManager &pushSetBlendFuncTaskRenderPass(Key<RenderPass> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
		ShadingManager &pushSetBlendFuncTaskRenderPass(Key<RenderPass> const &key, GLenum src, GLenum dest);
		ShadingManager &pushSetBlendConstantTaskRenderPass(Key<RenderPass> const &key, glm::vec4 const &blendPass);
		
		ShadingManager &popTaskRenderPass(Key<RenderPass> const &key);

		ShadingManager &draw(GLenum mode, Key<RenderPass> const &key, AGE::Drawable const *objectRender, size_t nbrObjectRender);


	private:
		std::map<Key<Shader>, Shader> _shaders;
		std::map<Key<UniformBlock>, UniformBlock> _uniformBlock;
		std::map<Key<Texture>, Texture *> _textures;
		std::map<Key<RenderPass>, RenderPass> _renderPass;
		std::map<Key<Material>, Material> _materials;

		std::pair<Key<Shader>, Shader *> _optimizeShaderSearch;
		std::pair<Key<UniformBlock>, UniformBlock *> _optimizeUniformBlockSearch;
		std::pair<Key<Texture>, Texture *> _optimizeTextureSearch;
		std::pair<Key<RenderPass>, RenderPass *> _optimizeRenderPassSearch;
		std::pair<Key<Material>, Material *> _optimizeMaterialSearch;

		// tool use in intern
		Shader *getShader(Key<Shader> const &key, std::string const &in);
		UniformBlock *getUniformBlock(Key<UniformBlock> const &key, std::string const &in);
		Texture *getTexture(Key<Texture> const &key, std::string const &in, GLenum type);
		RenderPass *getRenderPass(Key<RenderPass> const &key, std::string const &in);
		Material *getMaterial(Key<Material> const &key, std::string const &in);
	};

	template <typename TYPE>
	ShadingManager &ShadingManager::setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value)
	{
		UniformBlock *uniformBlock;

		if ((uniformBlock = getUniformBlock(key, "setUniformBlock")) == NULL)
			return (*this);
		uniformBlock->set<TYPE>(index, value);
		return (*this);
	}

	template <typename TYPE> 
	ShadingManager &ShadingManager::setMaterial(Key<Material> const &key, typename TYPE::return_type const &value)
	{
		Material *material;

		if ((material = getMaterial(key, "setMaterial")) == NULL)
			return (*this);
		material->set<TYPE>(value);
		return (*this);
	}

	template <typename TYPE> 
	typename TYPE::return_type ShadingManager::getMaterial(Key<Material> const &key)
	{
		Material *material;

		material = getMaterial(key, "getMaterial");
		return (material->get<TYPE>());
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