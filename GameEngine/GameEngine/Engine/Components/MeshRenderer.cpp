#include <Components/MeshRenderer.hh>
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <assert.h>
#include <Core/PrepareRenderThread.hpp>

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
		_scene(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	MeshRenderer::MeshRenderer(MeshRenderer &&o)
		: ComponentBase<MeshRenderer>(std::move(o)),
		_scene(o._scene),
		_key(o._key)
	{
	}

	MeshRenderer &MeshRenderer::operator=(MeshRenderer &&o)
	{
		_scene = o._scene;
		_key = o._key;
		return *this;
	}

	void MeshRenderer::init(AScene *scene, std::shared_ptr<AGE::MeshInstance> r)
	{
		_scene = scene;
		_key = scene->getInstance<AGE::Threads::Prepare>()->addMesh();
		scene->getLink(entityId)->registerOctreeObject(_key);
		assert(!_key.invalid());
		setMesh(r);
	}

	void MeshRenderer::reset(AScene *scene)
	{
		assert(!_key.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_key);
		scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
		_key = AGE::PrepareKey();
	}

	MeshRenderer &MeshRenderer::setMesh(const std::shared_ptr<AGE::MeshInstance> &mesh)
	{
		_mesh = mesh;
		updateGeometry();
		return (*this);
	}

	std::shared_ptr<AGE::MeshInstance> MeshRenderer::getMesh()
	{
		return _mesh;
	}

	MeshRenderer &MeshRenderer::setMaterial(const std::shared_ptr<AGE::MaterialSetInstance> &material)
	{
		_material = material;
		updateGeometry();
		return (*this);
	}

	std::shared_ptr<AGE::MaterialSetInstance> MeshRenderer::getMaterial()
	{
		return _material;
	}

	void MeshRenderer::updateGeometry()
	{
		assert(_scene != nullptr);

		if (this->_mesh == nullptr || this->_material == nullptr)
			return;
		assert(_material->datas.size() > 0);
		AGE::Vector<AGE::MaterialInstance> materials;
		for (auto &e : _mesh->subMeshs)
		{
			if (e.defaultMaterialIndex >= _material->datas.size())
				materials.push_back(_material->datas[0]);
			else
				materials.push_back(_material->datas[e.defaultMaterialIndex]);
		}
		_scene->getInstance<AGE::Threads::Prepare>()->updateGeometry(_key, _mesh->subMeshs, materials);
	}
}
