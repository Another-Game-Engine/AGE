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
#include <Render/RenderPass.hh>
#include <Render/Shader.hh>
#include <Core/Drawable.hh>

# undef DEBUG_MESSAGE
# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	assert(0 && std::string(std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "].").c_str()); return return_type; }

namespace gl
{
	struct Uniform;
	struct Sampler;
	struct InterfaceBlock;
	class Texture;
	class UniformBuffer;
	struct BindingRenderPass;
	struct BindingShader;

	class ShadingManager : public Dependency<ShadingManager>
	{
	public:
		GeometryManager geometryManager;

	public:
		ShadingManager();
		~ShadingManager();

		// shader handling
		Key<Shader> addPreShaderQuad();
		Key<Shader> addComputeShader(std::string const &compute);
		Key<Shader> addShader(std::string const &vert, std::string const &frag);
		Key<Shader> addShader(std::string const &geometry, std::string const &vert, std::string const &frag);
		ShadingManager &rmShader(Key<Shader> &shader);
		Key<Shader> getShader(size_t index) const;
		
		// uniform
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

		// sampler
		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		Key<Sampler> getShaderSampler(Key<Shader> const &shader, size_t index);
		ShadingManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Key<Texture> const &keytexture);

		// Interface
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> const &keyUniformBlock);
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
		//ShadingManager &setRenderPassMaterial(Key<Material> const &m, Key<RenderPass> const &r);
		template <typename TYPE> ShadingManager &setMaterial(Key<Material> const &key, typename TYPE::return_type const &value);
		template <typename TYPE> typename TYPE::return_type getMaterial(Key<Material> const &key);
		template <typename TYPE> ShadingManager &bindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		ShadingManager &unbindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		ShadingManager &bindTransformationToShader(Key<Shader> const &keyShader, Key<Uniform> const &keyUniform);

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
		//Key<RenderPass> addRenderPass(Key<Shader> const &shader);
		//ShadingManager &rmRenderPass(Key<RenderPass> &key);
		//Key<RenderPass> getRenderPass(size_t target) const;
		//ShadingManager &bindShaderRenderPass(Key<RenderPass> const &r, Key<Shader> const &s);
		//ShadingManager &bindInputRenderPass(Key<RenderPass> const &target, Key<RenderPass> const &input);
		//
		//ShadingManager &pushClearTaskRenderPass(Key<RenderPass> const &key, bool color = true, bool depth = true, bool stencil = false);
		//ShadingManager &pushSetClearValueTaskRenderPass(Key<RenderPass> const &key, glm::vec4 const &color, float depth = 1.0f, uint8_t stencil = 0);
		//ShadingManager &pushSetColorMaskTaskRenderPass(Key<RenderPass> const &key, glm::bvec4 const &color, GLuint index = 0);
		//ShadingManager &pushSetDepthMaskTaskRenderPass(Key<RenderPass> const &key, bool depth);
		//ShadingManager &pushSetStencilMaskTaskRenderPass(Key<RenderPass> const &key, uint8_t front, uint8_t back);
		//ShadingManager &pushSetTestTaskRenderPass(Key<RenderPass> const &key, bool scissor, bool stencil, bool depth);
		//ShadingManager &pushSetScissorTaskRenderPass(Key<RenderPass> const &key, glm::ivec4 const &area);
		//ShadingManager &pushSetStencilFunctionFrontFaceTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		//ShadingManager &pushSetStencilOperationFrontFaceTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		//ShadingManager &pushSetStencilFunctionBackFaceTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		//ShadingManager &pushSetStencilOperationBackFaceTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		//ShadingManager &pushSetStencilFunctionTaskRenderPass(Key<RenderPass> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		//ShadingManager &pushSetStencilOperationTaskRenderPass(Key<RenderPass> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		//ShadingManager &pushSetBlendEquationTaskRenderPass(Key<RenderPass> const &key, GLenum mode);
		//ShadingManager &pushSetBlendEquationTaskRenderPass(Key<RenderPass> const &key, GLenum colorMode, GLenum alphaMode);
		//ShadingManager &pushSetBlendFuncTaskRenderPass(Key<RenderPass> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
		//ShadingManager &pushSetBlendFuncTaskRenderPass(Key<RenderPass> const &key, GLenum src, GLenum dest);
		//ShadingManager &pushSetBlendConstantTaskRenderPass(Key<RenderPass> const &key, glm::vec4 const &blendPass);
		//ShadingManager &popTaskRenderPass(Key<RenderPass> const &key);
		//ShadingManager &configRenderPass(Key<RenderPass> const &renderPass, glm::ivec4 const &rect, GLint sample = 1);
		//ShadingManager &setModeRenderPass(Key<RenderPass> const &renderPass, GLenum mode);
		//ShadingManager &addColorOutputRenderPass(Key<RenderPass> const &renderPass, GLenum target, GLenum internalFormat);

		// Render
		//Key<Render> addRender(Key<Shader> const &shader);
		//ShadingManager &rmRender(Key<Render> &key);
		//Key<Render> getRender(size_t target) const;
		//ShadingManager &bindShaderRender(Key<Render> const &r, Key<Shader> const &s);
		//ShadingManager &bindInputRender(Key<Render> const &target, Key<RenderPass> const &input);

		//ShadingManager &pushClearTaskRender(Key<Render> const &key, bool color = true, bool depth = true, bool stencil = false);
		//ShadingManager &pushSetClearValueTaskRender(Key<Render> const &key, glm::vec4 const &color, float depth = 1.0f, uint8_t stencil = 0);
		//ShadingManager &pushSetColorMaskTaskRender(Key<Render> const &key, glm::bvec4 const &color, GLuint index = 0);
		//ShadingManager &pushSetDepthMaskTaskRender(Key<Render> const &key, bool depth);
		//ShadingManager &pushSetStencilMaskTaskRender(Key<Render> const &key, uint8_t front, uint8_t back);
		//ShadingManager &pushSetTestTaskRender(Key<Render> const &key, bool scissor, bool stencil, bool depth);
		//ShadingManager &pushSetScissorTaskRender(Key<Render> const &key, glm::ivec4 const &area);
		//ShadingManager &pushSetStencilFunctionFrontFaceTaskRender(Key<Render> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		//ShadingManager &pushSetStencilOperationFrontFaceTaskRender(Key<Render> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		//ShadingManager &pushSetStencilFunctionBackFaceTaskRender(Key<Render> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		//ShadingManager &pushSetStencilOperationBackFaceTaskRender(Key<Render> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		//ShadingManager &pushSetStencilFunctionTaskRender(Key<Render> const &key, GLenum func, int ref, uint8_t mask = 0xff);
		//ShadingManager &pushSetStencilOperationTaskRender(Key<Render> const &key, GLenum opStencilFail, GLenum opDepthFail, GLenum opDepthPass);
		//ShadingManager &pushSetBlendEquationTaskRender(Key<Render> const &key, GLenum mode);
		//ShadingManager &pushSetBlendEquationTaskRender(Key<Render> const &key, GLenum colorMode, GLenum alphaMode);
		//ShadingManager &pushSetBlendFuncTaskRender(Key<Render> const &key, GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);
		//ShadingManager &pushSetBlendFuncTaskRender(Key<Render> const &key, GLenum src, GLenum dest);
		//ShadingManager &pushSetBlendConstantTaskRender(Key<Render> const &key, glm::vec4 const &blendPass);
		//ShadingManager &popTaskRender(Key<Render> const &key);
		//ShadingManager &configRender(Key<Render> const &render, glm::ivec4 const &rect);
		//ShadingManager &setModeRender(Key<Render> const &render, GLenum mode);

		// drawing
		ShadingManager &draw(AGE::Vector<AGE::Drawable> const &objectRender);

	private:
		// all map
		std::map<Key<Shader>, Shader *> _shaders;
		std::map<Key<UniformBlock>, UniformBlock> _uniformBlock;
		std::map<Key<Texture>, Texture *> _textures;
		std::map<Key<Material>, Material> _materials;
		//std::map<Key<RenderPass>, size_t> _renderPass;
		//std::map<Key<Render>, size_t> _render;

		// optimize search in map
		std::pair<Key<Shader>, Shader *> _optimizeShaderSearch;
		std::pair<Key<UniformBlock>, UniformBlock *> _optimizeUniformBlockSearch;
		std::pair<Key<Texture>, Texture *> _optimizeTextureSearch;
		std::pair<Key<RenderPass>, size_t> _optimizeRenderPassSearch;
		std::pair<Key<Render>, size_t> _optimizeRenderSearch;
		std::pair<Key<Material>, Material *> _optimizeMaterialSearch;
		
		// binding
		AGE::Vector<BindingShader> _bindShader;
		AGE::Vector<BindingRenderPass> _bindRendering;

		// pool
		//AGE::Vector<RenderPass> _renderPassPool;
		//AGE::Vector<Render> _renderPool;

		// tool use in intern for search
		Shader *getShader(Key<Shader> const &key, std::string const &in);
		UniformBlock *getUniformBlock(Key<UniformBlock> const &key, std::string const &in);
		Texture *getTexture(Key<Texture> const &key, std::string const &in);
		//size_t getRenderPassIndex(Key<RenderPass> const &key, std::string const &in);
		//RenderPass *getRenderPass(Key<RenderPass> const &key, std::string const &in);
		size_t getRenderIndex(Key<Render> const &key, std::string const &in);
		Render *getRender(Key<Render> const &key, std::string const &in);
		Material *getMaterial(Key<Material> const &key, std::string const &in);
		
		// tool use in intern for bind shader to render/renderPass
		//void addBindShaderToRendering(Render *r, Shader *s);
		//void unbindShaderToRendering(Shader *s);

		// tool use in intern for bind render and renderPass
		//void addBindTargetToInput(Render *t, RenderPass *i);
		//void unbindRenderingTarget(Render *r);
		//void unbindRenderingInput(RenderPass *r);
	};

	struct BindingShader
	{
		Render *r;
		Shader *s;
		BindingShader(Render *r, Shader *s) : r(r), s(s)
		{}
	};

	struct BindingRenderPass
	{
		Render *target;
		RenderPass *input;
		BindingRenderPass(Render *target, RenderPass *input) : target(target), input(input)
		{}
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

	template <typename TYPE>
	ShadingManager &ShadingManager::bindMaterialToShader(Key<Shader> const &shaderKey, Key<Uniform> const &uniformKey)
	{
		Shader *shader;
		if ((shader = getShader(shaderKey, "bindMaterialToShader")) == NULL)
			return (*this);
		shader->bindingMaterial<TYPE>(uniformKey);
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