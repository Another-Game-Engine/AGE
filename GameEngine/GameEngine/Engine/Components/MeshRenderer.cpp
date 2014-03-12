#include "MeshRenderer.hh"
#include "Core/Engine.hh"
#include <Core/AScene.hh>	

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
		_shaderShadow("ShadowMapping"),
		mesh(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	void MeshRenderer::init(std::shared_ptr<AMediaFile> r)
	{
		mesh = std::static_pointer_cast<ObjFile>(r);
	}

	void MeshRenderer::init(std::shared_ptr<ObjFile> r)
	{
		mesh = r;
	}

	std::shared_ptr<ObjFile> const &MeshRenderer::getMesh() const
	{
		return (mesh);
	}

	void MeshRenderer::reset()
	{
		mesh = nullptr;
	}

	void MeshRenderer::render_shadowmap(glm::mat4 const &lightVP)
	{
		glEnable(GL_DEPTH_TEST);
		glm::mat4 depthMVP = lightVP * _entity->getGlobalTransform();
		OpenGLTools::UniformBuffer *lightMVP = _entity->getScene()->getEngine().getInstance<Renderer>()->getUniform("Light");
		auto shader = _entity->getScene()->getEngine().getInstance<Renderer>()->getShader(_shaderShadow);
		if (shader)
			shader->use();
		lightMVP->setUniform("lightMVP", depthMVP);
		lightMVP->flushChanges();
		for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
	}

	void MeshRenderer::render(std::function<void(OpenGLTools::Shader&)> func)
	{
		OpenGLTools::UniformBuffer *perModelUniform = _entity->getScene()->getInstance<Renderer>()->getUniform("PerModel");
		OpenGLTools::UniformBuffer *materialUniform = _entity->getScene()->getInstance<Renderer>()->getUniform("MaterialBasic");
		auto s = _entity->getScene()->getEngine().getInstance<Renderer>()->getShader(shader);
		if (s)
			s->use();
		func(*s);
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->material->materials[i].setUniforms(materialUniform);
			materialUniform->flushChanges();
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

	void MeshRenderer::render(bool shadow, GLuint shadowTex, glm::mat4 const &lightVP)
	{
		if (shadow)
		{
			glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
								 0.0, 0.5, 0.0, 0.0,
								 0.0, 0.0, 0.5, 0.0,
								 0.5, 0.5, 0.5, 1.0);
			OpenGLTools::UniformBuffer *perModelUniform = _entity->getScene()->getEngine().getInstance<Renderer>()->getUniform("PerModel");
			OpenGLTools::UniformBuffer *materialUniform = _entity->getScene()->getEngine().getInstance<Renderer>()->getUniform("MaterialBasic");
			OpenGLTools::UniformBuffer *shadowUniform = _entity->getScene()->getEngine().getInstance<Renderer>()->getUniform("LightBias");
			auto s = _entity->getScene()->getEngine().getInstance<Renderer>()->getShader(shader);
			if (s)
				s->use();
			glm::mat4 depthMVP = lightVP * _entity->getGlobalTransform();
			perModelUniform->setUniform("model", _entity->getGlobalTransform());
			perModelUniform->flushChanges();
			shadowUniform->setUniform("lightBiasMVP", biasMatrix * depthMVP);
			shadowUniform->flushChanges();
			for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
			{
				mesh->material->materials[i].setUniforms(materialUniform);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, shadowTex);
				materialUniform->flushChanges();
				mesh->geometries[i].buffer.draw(GL_TRIANGLES);
			}
		}
		else
		{
			OpenGLTools::UniformBuffer *perModelUniform = _entity->getScene()->getEngine().getInstance<Renderer>()->getUniform("PerModel");
			OpenGLTools::UniformBuffer *materialUniform = _entity->getScene()->getEngine().getInstance<Renderer>()->getUniform("MaterialBasic");
			auto s = _entity->getScene()->getEngine().getInstance<Renderer>()->getShader(shader);
			if (s)
				s->use();
			perModelUniform->setUniform("model", _entity->getGlobalTransform());
			perModelUniform->flushChanges();
			for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
			{
				mesh->material->materials[i].setUniforms(materialUniform);
				materialUniform->flushChanges();
				mesh->geometries[i].buffer.draw(GL_TRIANGLES);
			}
		}
	}

	std::string const &MeshRenderer::getShaderShadowMap() const
	{
		return (_shaderShadow);
	}

	void MeshRenderer::setShaderShadowMap(std::string const &shaderShadow)
	{
		_shaderShadow = shaderShadow;
	}
	void MeshRenderer::renderRaw()
	{
		OpenGLTools::UniformBuffer *perModelUniform = _entity->getScene()->getInstance<Renderer>()->getUniform("PerModel");

		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

}
