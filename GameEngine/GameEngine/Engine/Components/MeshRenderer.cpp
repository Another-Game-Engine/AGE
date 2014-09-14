#include <Components/MeshRenderer.hh>
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <assert.h>

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
		PrepareElement(),
		mesh(nullptr),
		material(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	MeshRenderer::MeshRenderer(MeshRenderer &&o)
		: ComponentBase<MeshRenderer>(std::move(o))
		, PrepareElement(std::move(o)),
		mesh(std::move(o.mesh)),
		material(std::move(o.material))
	{
	}

	MeshRenderer &MeshRenderer::operator=(MeshRenderer &&o)
	{
		PrepareElement::operator=(std::move(o));
		mesh = std::move(o.mesh);
		material = std::move(o.material);
		return *this;
	}

	void MeshRenderer::init(AScene *scene, std::shared_ptr<AGE::MeshInstance> r)
	{
		initOctree(scene, entityId);
		setMesh(r);
	}

	void MeshRenderer::reset(AScene *scene)
	{
		resetOctree(scene, entityId);
		mesh = nullptr;
		material = nullptr;
	}

	MeshRenderer &MeshRenderer::setMesh(const std::shared_ptr<AGE::MeshInstance> &_mesh)
	{
		mesh = _mesh;
		updateOctree();
		return (*this);
	}

	std::shared_ptr<AGE::MeshInstance> MeshRenderer::getMesh()
	{
		return mesh;
	}

	MeshRenderer &MeshRenderer::setMaterial(const std::shared_ptr<AGE::MaterialSetInstance> &_material)
	{
		material = _material;
		updateOctree();
		return (*this);
	}

	std::shared_ptr<AGE::MaterialSetInstance> MeshRenderer::getMaterial()
	{
		return material;
	}

	MeshRenderer &MeshRenderer::updateOctree()
	{
		assert(_scene != nullptr);

		if (this->mesh == nullptr || this->material == nullptr)
			return (*this);
		assert(material->datas.size() > 0);
		AGE::Vector<AGE::MaterialInstance> materials;
		for (auto &e : mesh->subMeshs)
		{
			if (e.defaultMaterialIndex >= material->datas.size())
				materials.push_back(material->datas[0]);
			else
				materials.push_back(material->datas[e.defaultMaterialIndex]);
		}
		_scene->getInstance<AGE::Threads::Prepare>()->updateGeometry(_OTKey, mesh->subMeshs, materials);
		return (*this);
	}

	AGE::PrepareElement &MeshRenderer::initOctree(::AScene *scene, ENTITY_ID entityId)
	{
		_scene = scene;
		_OTKey = scene->getInstance<AGE::Threads::Prepare>()->addMesh();
		scene->getLink(entityId)->registerOctreeObject(_OTKey);
		assert(!_OTKey.invalid());
		return (*this);
	}

	AGE::PrepareElement &MeshRenderer::resetOctree(::AScene *scene, ENTITY_ID entityId)
	{
		assert(!_OTKey.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_OTKey);
		scene->getInstance<AGE::Threads::Prepare>()->removeElement(_OTKey);
		_OTKey = AGE::PrepareKey();
		return (*this);
	}
}
