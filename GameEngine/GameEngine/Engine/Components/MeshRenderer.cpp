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
		// DEFAULT MATERIAL OF MESH COPY
		materials.clear();
		auto &m = mesh->getDefaultMaterialsList();
		for (auto &e : m)
		{
			materials.push_back(*(e.get()));
		}
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
		auto &g = mesh->getGeometry();
		auto &b = mesh->getBuffer();
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < g.size(); ++i)
		{
			materials[i].setUniforms(materialUniform);
			materialUniform->flushChanges();
			b[i].draw(GL_TRIANGLES);
		}
	}

	std::vector<Material> &MeshRenderer::getMaterials()
	{
		return materials;
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
