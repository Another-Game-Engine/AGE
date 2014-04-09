#include "MeshRenderer.hh"
#include "Core/Engine.hh"
#include <Core/AScene.hh>	

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
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

	void MeshRenderer::render(std::function<void(OpenGLTools::Shader&)> func)
	{
		auto perModelUniform = _entity->getScene().lock()->getInstance<Renderer>()->getUniform("PerModel");
		auto materialUniform = _entity->getScene().lock()->getInstance<Renderer>()->getUniform("MaterialBasic");
		auto s = _entity->getScene().lock()->getInstance<Renderer>()->getShader(shader);
		if (s)
			s->use();
		func(*s);
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
		for (std::size_t i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->material->materials[i].setUniforms(materialUniform);
			materialUniform->flushChanges();
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

	void MeshRenderer::renderRaw()
	{
		std::shared_ptr<OpenGLTools::UniformBuffer> perModelUniform = _entity->getScene().lock()->getInstance<Renderer>()->getUniform("PerModel");

		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

}
