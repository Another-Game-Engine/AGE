#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"
#include <ResourceManager/Texture.hh>
#include <ResourceManager/ResourceManager.hh>

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

	void MeshRenderer::init(std::string const &resource)
	{
		mesh = _entity->getScene()->getEngine().getInstance<Resources::ResourceManager>().getResource(resource);
	}

	SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
	{
		return (mesh);
	}

	void MeshRenderer::reset()
	{
		mesh = nullptr;
	}

	void MeshRenderer::render()
	{
		OpenGLTools::UniformBuffer *perModelUniform = _entity->getScene()->getEngine().getInstance<Renderer>().getUniform("PerModel");
		OpenGLTools::UniformBuffer *materialUniform = _entity->getScene()->getEngine().getInstance<Renderer>().getUniform("MaterialBasic");
		auto s = _entity->getScene()->getEngine().getInstance<Renderer>().getShader(shader);
		if (s)
			s->use();
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		auto &g = _mesh->getGeometry();
		auto &b = _mesh->getDrawable();
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < g.size(); ++i)
		{
			mesh->getMaterial()[i]->setUniforms(materialUniform);
			materialUniform->flushChanges();
			b[i].draw(GL_TRIANGLES);
		}
	}

	std::vector<SmartPointer<Material>> &MeshRenderer::getMaterials()
	{
		return mesh->getMaterial();
	}

	Material *MeshRenderer::getMaterial(const std::string &name)
	{
		for (auto &e : materials)
		{
			if (e.name == name)
				return &e;
		}
		return nullptr;
	}

	Material *MeshRenderer::getMaterial(unsigned int index)
	{
		if (index < materials.size())
			return &materials[index];
		return nullptr;
	}

}
