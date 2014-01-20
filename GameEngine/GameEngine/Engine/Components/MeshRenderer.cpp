#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"
#include <ResourceManager/Texture.hh>
#include <ResourceManager/ResourceManager.hh>

namespace Component
{
	MeshRenderer::MeshRenderer(AScene *scene, Entity &entity) :
		Component::ComponentBase<MeshRenderer>(scene, entity),
		_mesh(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
		reset();
	}

	void MeshRenderer::init(std::string const &resource)
	{
		_mesh = _scene->getEngine().getInstance<Resources::ResourceManager>().getResource(resource);
		// DEFAULT MATERIAL OF MESH COPY
		_materials.clear();
		auto &m = _mesh->getDefaultMaterialsList();
		for (auto &e : m)
		{
			_materials.push_back(*(e.get()));
		}
	}

	SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
	{
		return (_mesh);
	}

	void MeshRenderer::reset()
	{
		_mesh = nullptr;
	}

	void MeshRenderer::render()
	{
		OpenGLTools::UniformBuffer *perModelUniform = _scene->getEngine().getInstance<Renderer>().getUniform("PerModel");
		OpenGLTools::UniformBuffer *materialUniform = _scene->getEngine().getInstance<Renderer>().getUniform("MaterialBasic");
		auto shader = _scene->getEngine().getInstance<Renderer>().getShader(_shader);
		if (shader)
			shader->use();
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		auto &g = _mesh->getGeometry();
		auto &b = _mesh->getBuffer();
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < g.size(); ++i)
		{
			_materials[i].setUniforms(materialUniform);
			materialUniform->flushChanges();
			b[i].draw(GL_TRIANGLES);
		}
	}

	std::vector<Material> &MeshRenderer::getMaterials()
	{
		return _materials;
	}

	Material *MeshRenderer::getMaterial(const std::string &name)
	{
		for (auto &e : _materials)
		{
			if (e.name == name)
				return &e;
		}
		return nullptr;
	}

	Material *MeshRenderer::getMaterial(unsigned int index)
	{
		if (index < _materials.size())
			return &_materials[index];
		return nullptr;
	}

}
