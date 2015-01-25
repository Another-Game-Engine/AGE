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
#include <Render/Render.hh>
#include <Render/Shader.hh>
#include <Render/MacroRenderManager.hh>
#include <Core/CullableObjects.hh>
#include <Render/LocationStorage.hh>
#include <tmq/message.hpp>
#include <Render/Pipeline.hh>

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

	enum SimpleForm
	{
		QUAD = 0,
		SPHERE = 1
	};

	class RenderManager : public Dependency<RenderManager>
	{
	public:
		Key<VertexPool> simpleFormPoolGeo;
		Key<IndexPool> simpleFormPoolId;
		LocationStorage locationStorage;

	public:
		RenderManager();
		~RenderManager();
		RenderManager &createPreShaderQuad();
		Key<Shader> addComputeShader(std::string const &compute);
		Key<Shader> addShader(std::string const &vert, std::string const &frag);
		Key<Shader> addShader(std::string const &geometry, std::string const &vert, std::string const &frag);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat4 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::mat3 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, float value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::vec4 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, glm::vec3 const &value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, bool value);
		Key<Uniform> addShaderUniform(Key<Shader> const &shader, std::string const &flag, size_t sizeType, size_t size);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec3 const &vec3);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::vec4 const &vec4);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, float v);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat3 const &mat3);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &mat4);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, bool b);
		RenderManager &setShaderUniform(Key<Shader> const &shader, Key<Uniform> const &key, glm::mat4 const &data, size_t index);
		Key<Sampler> addShaderSampler(Key<Shader> const &shader, std::string const &flag);
		RenderManager &setShaderSampler(Key<Shader> const &shader, Key<Sampler> const &key, Key<Texture> const &keytexture);
		RenderManager &updateShader(Key<Shader> const &shader, glm::mat4 const &transform, Key<Material> const &mat);
		RenderManager &updateShader(Key<Shader> const &shader);
		Key<InterfaceBlock> addShaderInterfaceBlock(Key<Shader> const &shader, std::string const &flag, Key<UniformBlock> &keyUniformBlock);
		RenderManager &setShaderInterfaceBlock(Key<Shader> const &shader, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u);
		Key<UniformBlock> addUniformBlock();
		RenderManager &introspectionBlock(Key<Shader> const &s, Key<InterfaceBlock> const &i, Key<UniformBlock> const &u);
		template <typename TYPE> RenderManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value);
		template <typename TYPE> RenderManager &setUniformBlock(Key<UniformBlock> const &key, size_t index, TYPE const &value, size_t indexTab);
		RenderManager &bindTransformationToShader(Key<Shader> const &keyShader, Key<Uniform> const &keyUniform);
		template <typename TYPE> RenderManager &bindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		template <typename TYPE> RenderManager &bindMaterialToShader(Key<Shader> const &s, Key<Sampler> const &u);
		RenderManager &unbindMaterialToShader(Key<Shader> const &s, Key<Uniform> const &u);
		Key<Material> getDefaultMaterial();
		Key<Texture> getDefaultTexture2D();
		Key<Material> addMaterial();
		template <typename TYPE> RenderManager &setMaterial(Key<Material> const &key, typename TYPE::return_type const &value);
		template <typename TYPE> RenderManager &setMaterial(Key<Material> const &key, Key<Texture> const &key_tex);
		template <typename TYPE> typename TYPE::return_type getMaterial(Key<Material> const &key);
		RenderManager &setShaderByMaterial(Key<Shader> &shader, Key<Material> const &key);
		Key<Texture> addTexture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipmapping);
		RenderManager &uploadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		RenderManager &uploadTextureMipMaps(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		RenderManager &downloadTexture(Key<Texture> const &key, GLenum format, GLenum type, GLvoid *img);
		RenderManager &setlevelTargetTexture(Key<Texture> const &key, uint8_t level);
		RenderManager &bindTexture(Key<Texture> const &key);
		RenderManager &unbindTexture(Key<Texture> const &key);
		RenderManager &configUploadTexture2D(Key<Texture> const &key, glm::ivec4 const &rect);
		RenderManager &parameterTexture(Key<Texture> const &key, GLenum pname, GLint param);
		GLenum getTypeTexture(Key<Texture> const &key);
		Key<RenderPass> addRenderPass(Key<Shader> const &shader, glm::ivec4 const &rect);
		RenderManager &pushDrawTaskRenderBuffer(Key<RenderPass> const &key);
		GEN_DEC_RENDER_PUSH_TASK(RenderPass);
		GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(RenderPass);
		GEN_DEC_DRAWABLERENDER_PUSH_TASK(RenderPass);
		Key<RenderPostEffect> addRenderPostEffect(Key<Shader> const &s, glm::ivec4 const &rect);
		GEN_DEC_RENDER_PUSH_TASK(RenderPostEffect);
		GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(RenderPostEffect);
		GEN_DEC_DRAWABLERENDER_PUSH_TASK(RenderPostEffect);
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPass> const &renderPass);
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect, Key<RenderPostEffect> const &renderPostEffect);
		Key<RenderOnScreen> addRenderOnScreen(glm::ivec4 const &rect, Key<EmptyRenderPass> const &renderPostEffect);
		GEN_DEC_RENDER_PUSH_TASK(RenderOnScreen);
		RenderManager &configRenderOnScreen(Key<RenderOnScreen> const &renderOnScreen, glm::ivec4 const &rect, GLenum mode);
		Key<EmptyRenderPass> addEmptyRenderPass(glm::ivec4 const &rect);
		GEN_DEC_RENDER_PUSH_TASK(EmptyRenderPass);
		GEN_DEC_RENDEROFFSCREEN_PUSH_TASK(EmptyRenderPass);
		Key<Pipeline> addPipeline();
		RenderManager &pushRenderPassPipeline(Key<Pipeline> const &p, Key<RenderPass> const &r);
		RenderManager &pushRenderPostEffectPipeline(Key<Pipeline> const &p, Key<RenderPostEffect> const &r);
		RenderManager &pushRenderOnScreenPipeline(Key<Pipeline> const &p, Key<RenderOnScreen> const &r);
		RenderManager &pushEmptyRenderPassPipeline(Key<Pipeline> const &p, Key<EmptyRenderPass> const &r);
		RenderManager &configPipeline(Key<Pipeline> const &key, DrawType type);
		RenderManager &updatePipeline(Key<Pipeline> const &p, AGE::Vector<AGE::Drawable> const &objectRender);
		RenderManager &drawPipelines();
		RenderManager &drawPipeline(Key<Pipeline> const &key, AGE::Vector<AGE::Drawable> const &objectRender);
		RenderManager &draw(Key<RenderOnScreen> const &key, Key<RenderPass> const &r, AGE::Vector<AGE::Drawable> const &objectRender);
	
		RenderManager &createQuadSimpleForm();
		RenderManager &createSphereSimpleForm();
		Key<Vertices> getSimpleFormGeo(SimpleForm form);
		Key<Indices> getSimpleFormId(SimpleForm form);
		Key<VertexPool> addVertexPool(uint8_t nbrAttributes, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t>const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent);
		Key<IndexPool> addIndexPool();
		Key<Vertices> addVertices(size_t nbrVertices, AGE::Vector<size_t> const &sizeBuffers, AGE::Vector<void *> const &buffers, Key<VertexPool> const &pool);
		Key<Indices> addIndices(size_t nbrIndices, AGE::Vector<uint32_t> const &buffers, Key<IndexPool> const &pool);
		RenderManager &rmVertices(Key<VertexPool> const &vertexPool, Key<Vertices> &vertices);
		RenderManager &rmIndices(Key<IndexPool> const &indexPool, Key<Indices> &indices);
		static void generateIcoSphere(size_t recursion, AGE::Vector<void *> &vertex, AGE::Vector<unsigned int> &index, size_t &nbrElementId, size_t &nbrElementGeo);
		RenderManager &draw(GLenum mode, Key<Vertices> const &vertices, Key<VertexPool> const &pool);
		RenderManager &draw(GLenum mode, Key<Indices> const &indices, Key<Vertices> const &vertices, Key<IndexPool> const &indexPool, Key<VertexPool> const &vertexPool);

	private:
		std::map<SimpleForm, Key<Vertices>> _simpleFormGeo;
		std::map<SimpleForm, Key<Indices>> _simpleFormId;
		Shader * _preShaderQuad;
		AGE::Vector<Shader *> _shaders;
		AGE::Vector<UniformBlock *> _uniformBlock;
		AGE::Vector<Texture *> _textures;
		AGE::Vector<Material> _materials;
		AGE::Vector<Pipeline> _pipelines;
		AGE::Vector<RenderPass *> _renderPass;
		AGE::Vector<RenderPostEffect *> _renderPostEffect;
		AGE::Vector<RenderOnScreen *> _renderOnScreen;
		AGE::Vector<EmptyRenderPass *> _emptyRenderPass;
		AGE::Vector<IndexPool *> _indexPool;
		AGE::Vector<VertexPool *> _vertexPool;
	
		bool _defaultMaterialCreated;
		Key<Material> _defaultMaterial;
		bool _defaultTexture2DCreated;
		Key<Texture> _defaultTexture2D;
		size_t _renderManagerNumber;

	private:
		void initSimpleForm();
		Shader *getShader(Key<Shader> const &key){ assert(!!key); return (_shaders[key.getId()]); }
		UniformBlock *getUniformBlock(Key<UniformBlock> const &key)	{ assert(!!key); return (_uniformBlock[key.getId()]); }
		Texture *getTexture(Key<Texture> const &key) { assert(!!key); return (_textures[key.getId()]);}
		Material *getMaterial(Key<Material> const &key) { assert(!!key); return (&_materials[key.getId()]);}
		Pipeline *getPipeline(Key<Pipeline> const &key) { assert(!!key); return (&_pipelines[key.getId()]); }
		EmptyRenderPass *getEmptyRenderPass(Key<EmptyRenderPass> const &key) { assert(!!key); return (_emptyRenderPass[key.getId()]); }
		RenderPass *getRenderPass(Key<RenderPass> const &key) { assert(!!key); return (_renderPass[key.getId()]); }
		RenderOnScreen *getRenderOnScreen(Key<RenderOnScreen> const &key) { assert(!!key); return (_renderOnScreen[key.getId()]); }
		RenderPostEffect *getRenderPostEffect(Key<RenderPostEffect> const &key) { assert(!!key); return (_renderPostEffect[key.getId()]); }
		VertexPool *getVertexPool(Key<VertexPool> const &key) { assert(!!key); return (_vertexPool[key.getId()]); }
		IndexPool *getIndexPool(Key<IndexPool> const &key){ assert(!!key); return (_indexPool[key.getId()]); };
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
		Material *material = getMaterial(key);
		material->set<TYPE>(value);
		return (*this);
	}

	template <typename TYPE>
	RenderManager &RenderManager::setMaterial(Key<Material> const &key, Key<Texture> const &key_tex)
	{
		Texture *texture = getTexture(key_tex);
		Material *material = getMaterial(key);
		material->set<TYPE>(texture->getId());
		return (*this);
	}

	template <typename TYPE>
	typename TYPE::return_type RenderManager::getMaterial(Key<Material> const &key)
	{
		Material *material = getMaterial(key);
		return (material.get<TYPE>();)
	}
}