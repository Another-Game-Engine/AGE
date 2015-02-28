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
		, _scene(nullptr)
		, _serializationInfos(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}


	void MeshRenderer::init(AScene *scene
		, std::shared_ptr<AGE::MeshInstance> mesh /* = nullptr */
		, std::shared_ptr<AGE::MaterialSetInstance> material /*= nullptr*/)
	{
		_scene = scene;
		if (mesh && material)
		{
			setMeshAndMaterial(mesh, material);
		}
	}

	void MeshRenderer::reset(AScene *scene)
	{
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
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<Tasks::MainToPrepare::SetMeshMaterial>(_material, _mesh);
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
		AGE::GetPrepareThread()->updateGeometry(_key, _mesh->subMeshs);
	}

	void MeshRenderer::postUnserialization(AScene *scene)
	{
		_scene = scene;
		if (_serializationInfos)
		{
			if (!_serializationInfos->mesh.empty())
			{
				scene->getInstance<AGE::AssetsManager>()->loadMesh(_serializationInfos->mesh
					, { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }
				, "WE_MESH_LOADING");

				std::size_t totalToLoad = 0;
				std::size_t	toLoad = 0;
				std::string loadingError;
				do {
					scene->getInstance<AGE::AssetsManager>()->updateLoadingChannel("WE_MESH_LOADING", totalToLoad, toLoad, loadingError);
				} while
					(toLoad != 0 && loadingError.size() == 0);

				_mesh = _scene->getInstance<AGE::AssetsManager>()->getMesh(_serializationInfos->mesh);
			}
			if (!_serializationInfos->material.empty())
			{
				scene->getInstance<AGE::AssetsManager>()->loadMaterial(_serializationInfos->material
				, "WE_MESH_LOADING");

				std::size_t totalToLoad = 0;
				std::size_t	toLoad = 0;
				std::string loadingError;
				do {
					scene->getInstance<AGE::AssetsManager>()->updateLoadingChannel("WE_MESH_LOADING", totalToLoad, toLoad, loadingError);
				} while
					(toLoad != 0 && loadingError.size() == 0);

				_material = _scene->getInstance<AGE::AssetsManager>()->getMaterial(_serializationInfos->material);
			}
			if (_mesh && _material)
			{
				setMeshAndMaterial(_mesh, _material);
			}
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
				scene->getInstance<AGE::AssetsManager>()->loadMesh(OldFile(selectedMeshPath), { AGE::MeshInfos::Positions, AGE::MeshInfos::Normals, AGE::MeshInfos::Uvs, AGE::MeshInfos::Tangents }, "WE_MESH_LOADING");

				std::size_t totalToLoad = 0;
				std::size_t	toLoad = 0;
				std::string loadingError;
				do {
					scene->getInstance<AGE::AssetsManager>()->updateLoadingChannel("WE_MESH_LOADING", totalToLoad, toLoad, loadingError);
				} while
					(toLoad != 0 && loadingError.size() == 0);
			}
			_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(selectedMeshPath);
			AGE_ASSERT(_mesh != nullptr);
			if (_material)
			{
				setMeshAndMaterial(_mesh, _material);
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
				scene->getInstance<AGE::AssetsManager>()->loadMaterial(OldFile(selectedMaterialPath), "WE_MESH_LOADING");

				std::size_t totalToLoad = 0;
				std::size_t	toLoad = 0;
				std::string loadingError;
				do {
					scene->getInstance<AGE::AssetsManager>()->updateLoadingChannel("WE_MESH_LOADING", totalToLoad, toLoad, loadingError);
				} while
					(toLoad != 0 && loadingError.size() == 0);
			}
			_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(selectedMaterialPath);
			AGE_ASSERT(_material != nullptr);
			if (_mesh)
			{
				setMeshAndMaterial(_mesh, _material);
			}
		}
		//ImGui::ListBoxFooter();
		ImGui::PopItemWidth();

	}
#endif
}