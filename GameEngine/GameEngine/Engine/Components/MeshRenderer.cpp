#include <Components/MeshRenderer.hh>
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/AssetManager.hh>
#include <assert.h>
#include <Threads/ThreadManager.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>

namespace AGE
{

	MeshRenderer::MeshRenderer() :
		ComponentBase(),
		_scene(nullptr)
		, _serializationInfos(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}


	void MeshRenderer::init(AScene *scene, std::shared_ptr<AGE::MeshInstance> r /* = nullptr */)
	{
		_scene = scene;
		setMesh(r);
	}

	void MeshRenderer::reset(AScene *scene)
	{
		if (!_key.invalid())
			entity.getLink().unregisterOctreeObject(_key);
		//scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
		_key = AGE::PrepareKey();
	}

	MeshRenderer &MeshRenderer::setMesh(const std::shared_ptr<AGE::MeshInstance> &mesh)
	{
		if (!mesh)
			return (*this);
		_mesh = mesh;
		_key = AGE::GetPrepareThread()->addMesh();
		entity.getLink().registerOctreeObject(_key);
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
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<Tasks::Render::SetMeshMaterial>(_material, _mesh);
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
			AGE::GetPrepareThread()->updateGeometry(_key, _mesh->subMeshs);
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
				// todo with animations
			}
		}
	}

}