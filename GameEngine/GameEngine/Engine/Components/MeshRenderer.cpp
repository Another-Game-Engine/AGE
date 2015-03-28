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
#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#include <AssetManagement/AssetManager.hh>
#include <Utils/Debug.hpp>
#endif

namespace AGE
{
	MeshRenderer::MeshRenderer() :
		ComponentBase()
		, _serializationInfos(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}


	void MeshRenderer::init(std::shared_ptr<AGE::MeshInstance> mesh /* = nullptr */
		, std::shared_ptr<AGE::MaterialSetInstance> material /*= nullptr*/)
	{
		if (mesh && material)
		{
			setMeshAndMaterial(mesh, material);
		}
	}

	void MeshRenderer::reset()
	{
		_mesh = nullptr;
		_material = nullptr;
		if (!_key.invalid())
		{
			entity.getLink().unregisterOctreeObject(_key);
		}
		//scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
		_key = AGE::PrepareKey();
	}

	bool MeshRenderer::setMeshAndMaterial(
		const std::shared_ptr<AGE::MeshInstance> &mesh,
		const std::shared_ptr<AGE::MaterialSetInstance> &material)
	{
		if (!mesh || !material)
		{
			return false;
		}
		if (!_key.invalid())
		{
			entity.getLink().unregisterOctreeObject(_key);
		}

		//create key
		_key = AGE::GetPrepareThread()->addMesh();
		entity.getLink().registerOctreeObject(_key);

		_mesh = mesh;
		_material = material;
		_updateGeometry();
		//AGE::GetPrepareThread()->getQueue()->emplaceCommand<Tasks::MainToPrepare::SetMeshMaterial>(_material, _mesh);
		return true;
	}

	void MeshRenderer::enableMode(RenderModes mode)
	{
		assert(_mesh != nullptr);
		for (auto &subMesh : _mesh->subMeshs)
		{
			subMesh.renderMode[mode] = true;
		}
		_updateGeometry();
	}

	void MeshRenderer::disableMode(RenderModes mode)
	{
		assert(_mesh != nullptr);
		for (auto &subMesh : _mesh->subMeshs)
		{
			subMesh.renderMode[mode] = false;
		}
		_updateGeometry();
	}

	void MeshRenderer::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const MeshRenderer*>(model);

		_material = o->_material;
		_mesh = o->_mesh;
#ifdef EDITOR_ENABLED
		selectedMeshIndex = o->selectedMeshIndex;
		selectedMeshName = o->selectedMeshName;
		selectedMeshPath = o->selectedMeshPath;

		selectedMaterialIndex = o->selectedMaterialIndex;
		selectedMaterialName = o->selectedMaterialName;
		selectedMaterialPath = o->selectedMaterialPath;
#endif
		setMeshAndMaterial(_mesh, _material);
	}

	std::shared_ptr<AGE::MeshInstance> MeshRenderer::getMesh()
	{
		return _mesh;
	}

	std::shared_ptr<AGE::MaterialSetInstance> MeshRenderer::getMaterial()
	{
		return _material;
	}

	void MeshRenderer::_updateGeometry()
	{
		if (_mesh == nullptr
			|| _material == nullptr)
		{
			return;
		}
		AGE::GetPrepareThread()->updateGeometry(_key, _mesh->subMeshs, _material->datas);
	}

	void MeshRenderer::postUnserialization()
	{
		auto scene = entity.getScene();
		
		if (_serializationInfos)
		{
			_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(_serializationInfos->mesh);
			if (!_serializationInfos->mesh.empty() && !_mesh)
			{
				scene->getInstance<AGE::AssetsManager>()->pushNewCallback(_serializationInfos->mesh,
					std::function<void()>([=](){
					_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(_serializationInfos->mesh);
					setMeshAndMaterial(_mesh, _material);
				}));
				scene->getInstance<AGE::AssetsManager>()->loadMesh(_serializationInfos->mesh
					, _serializationInfos->mesh);
			}

			_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(_serializationInfos->material);
			if (!_serializationInfos->material.empty() && !_material)
			{
				scene->getInstance<AGE::AssetsManager>()->pushNewCallback(_serializationInfos->material,
					std::function<void()>([=](){
					_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(_serializationInfos->material);
					setMeshAndMaterial(_mesh, _material);
				}));
				scene->getInstance<AGE::AssetsManager>()->loadMaterial(_serializationInfos->material, _serializationInfos->material);
			}

			setMeshAndMaterial(_mesh, _material);
#ifdef EDITOR_ENABLED
			selectedMaterialPath = _serializationInfos->material;
			selectedMeshPath = _serializationInfos->mesh;
#endif
		}
	}

#ifdef EDITOR_ENABLED
	void MeshRenderer::editorCreate(AScene *scene)
	{}

	void MeshRenderer::editorDelete(AScene *scene)
	{}

	void MeshRenderer::editorUpdate(AScene *scene)
	{
		if ((*meshPathList)[selectedMeshIndex] != selectedMeshPath)
		{
			std::size_t i = 0;
			for (auto &e : *meshPathList)
			{
				if (e == selectedMeshPath)
				{
					selectedMeshIndex = i;
					break;
				}
				++i;
			}
		}
		if ((*materialPathList)[selectedMaterialIndex] != selectedMaterialPath)
		{
			std::size_t i = 0;
			for (auto &e : *materialPathList)
			{
				if (e == selectedMaterialPath)
				{
					selectedMaterialIndex = i;
					break;
				}
				++i;
			}
		}

		ImGui::PushItemWidth(-1);
		if (ImGui::ListBox("Meshs", (int*)&selectedMeshIndex, &(meshFileList->front()), (int)(meshFileList->size())))
		{
			selectedMeshName = (*meshFileList)[selectedMeshIndex];
			selectedMeshPath = (*meshPathList)[selectedMeshIndex];

			_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(selectedMeshPath);

			if (!_mesh)
			{
				scene->getInstance<AGE::AssetsManager>()->pushNewCallback(selectedMeshPath,
					std::function<void()>([=](){
					_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(selectedMeshPath);
					if (_mesh && _material)
					{
						setMeshAndMaterial(_mesh, _material);
					}
				}));
				scene->getInstance<AGE::AssetsManager>()->loadMesh(OldFile(selectedMeshPath), selectedMeshPath);
			}
			else
			{
				if (_mesh && _material)
				{
					setMeshAndMaterial(_mesh, _material);
				}
			}
		}
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(-1);
		if (!materialFileList->empty() && ImGui::ListBox("Material", (int*)&selectedMaterialIndex, &(materialFileList->front()), (int)(materialFileList->size())))
		{
			selectedMaterialName = (*materialFileList)[selectedMaterialIndex];
			selectedMaterialPath = (*materialPathList)[selectedMaterialIndex];

			_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(selectedMaterialPath);

			if (!_material)
			{
				scene->getInstance<AGE::AssetsManager>()->pushNewCallback(selectedMaterialPath,
					std::function<void()>([=](){
					_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(selectedMaterialPath);
					if (_mesh && _material)
					{
						setMeshAndMaterial(_mesh, _material);
					}
				}));
				scene->getInstance<AGE::AssetsManager>()->loadMaterial(OldFile(selectedMaterialPath), selectedMaterialPath);
			}
			else
			{
				if (_mesh && _material)
				{
					setMeshAndMaterial(_mesh, _material);
				}
			}
		}
		//ImGui::ListBoxFooter();
		ImGui::PopItemWidth();

	}
#endif
}