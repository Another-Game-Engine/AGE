#include <Components/MeshRenderer.hh>
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <assert.h>
#include <Core/PrepareRenderThread.hpp>
#include <Core/AssetsManager.hpp>

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
		_scene(nullptr)
		, _serializationInfos(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	MeshRenderer::MeshRenderer(MeshRenderer &&o)
		: ComponentBase<MeshRenderer>(std::move(o)),
		_scene(o._scene),
		_key(o._key),
		_serializationInfos(std::move(o._serializationInfos)),
		_material(o._material),
		_mesh(o._mesh)
	{
	}

	MeshRenderer &MeshRenderer::operator=(MeshRenderer &&o)
	{
		_scene = o._scene;
		_key = o._key;
		_serializationInfos = std::move(o._serializationInfos);
		_material = o._material;
		_mesh = o._mesh;
		return *this;
	}

	void MeshRenderer::init(AScene *scene, std::shared_ptr<AGE::MeshInstance> r)
	{
		_scene = scene;
		setMesh(r);
	}

	void MeshRenderer::reset(AScene *scene)
	{
		assert(!_key.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_key);
		//scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
		_key = AGE::PrepareKey();
	}

	MeshRenderer &MeshRenderer::setMesh(const std::shared_ptr<AGE::MeshInstance> &mesh)
	{
		_mesh = mesh;
		_key = _scene->getInstance<AGE::Threads::Prepare>()->addMesh();
		_scene->getLink(entityId)->registerOctreeObject(_key);
		assert(!_key.invalid());
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

	MeshRenderer &MeshRenderer::setAnimation(const gl::Key<AGE::AnimationInstance> &key)
	{
		_animation = key;
		updateGeometry();
		return (*this);
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
		_scene->getInstance<AGE::Threads::Prepare>()->updateGeometry(_key, _mesh->subMeshs, materials, _animation);
	}

	void MeshRenderer::postUnserialization(AScene *scene)
	{
		_scene = scene;
		if (_serializationInfos)
		{
			if (!_serializationInfos->mesh.empty())
			{
				auto mesh = _scene->getInstance<AGE::AssetsManager>()->getMesh(_serializationInfos->mesh);
				if (mesh)
				{
					init(_scene, mesh);
				}
			}
			if (!_serializationInfos->material.empty())
			{
				auto material = _scene->getInstance<AGE::AssetsManager>()->getMaterial(_serializationInfos->material);
				if (material)
				{
					setMaterial(material);
				}
			}
			// todo with animations
		}
	}


}
