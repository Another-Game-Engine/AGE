#include <ComponentsCore/MeshRenderer.hh>
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/AssetManager.hh>
#include <assert.h>
#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#include <AssetManagement/AssetManager.hh>
#include <Utils/Debug.hpp>
#endif
#include "Graphic\DRBMeshData.hpp"
#include "Graphic/DRBSkinnedMesh.hpp"

//tmp
#include "Configuration.hpp"

//BFC
#include "Graphic/GraphicElementManager.hpp"

// tmp
#include <Skinning/AnimationManager.hpp>

namespace AGE
{
	MeshRenderer::MeshRenderer() :
		ComponentBase()
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
		_renderMode.reset();
		_resetDrawableHandle();
	}

	bool MeshRenderer::setMeshAndMaterial(
		const std::shared_ptr<AGE::MeshInstance> &mesh,
		const std::shared_ptr<AGE::MaterialSetInstance> &material)
	{
		if (!mesh || !material)
		{
			return false;
		}
		_meshPath = mesh->path;
		_materialPath = material->path;
		_mesh = mesh;
		_material = material;
		_updateGeometry();
		return true;
	}

	void MeshRenderer::enableRenderMode(RenderModes mode)
	{
		_renderMode[mode] = true;
	}

	void MeshRenderer::disableRenderMode(RenderModes mode)
	{
		_renderMode[mode] = false;
	}

	void MeshRenderer::setSkinningMatrix(std::size_t size)
	{
		SCOPE_profile_cpu_function("Animations");

		if (_drawableHandle.invalid())
			return;
		bool success = false;
		for (auto &handle : _drawableHandle.getHandles())
		{
			if (handle.getPtr<DRBMesh>()->getDatas()->hadRenderMode(RenderModes::AGE_SKINNED))
			{
				handle.getPtr<DRBSkinnedMesh>()->setSkinningMatrix(size);
				success = true;
			}
		}
		AGE_ASSERT(success, "You tried to update skinning matrix of a non skinned mesh.");
	}

	void MeshRenderer::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const MeshRenderer*>(model);

		_material = o->_material;
		_mesh = o->_mesh;
		_renderMode = o->_renderMode;
		_materialPath = o->_materialPath;
		_meshPath = o->_meshPath;
#ifdef EDITOR_ENABLED
		selectedMeshIndex = o->selectedMeshIndex;
		selectedMeshName = o->selectedMeshName;

		selectedMaterialIndex = o->selectedMaterialIndex;
		selectedMaterialName = o->selectedMaterialName;
#endif
		setMeshAndMaterial(_mesh, _material);
		postUnserialization();
	}

	void MeshRenderer::_updateGeometry()
	{
		if (_mesh == nullptr
			|| _material == nullptr)
		{
			return;
		}
		auto manager = entity->getScene()->getInstance<GraphicElementManager>();
		_resetDrawableHandle();
		_drawableHandle = manager->addMesh(_mesh, _material);
		
		for (auto &handle : _drawableHandle.getHandles())
		{
			handle.getPtr<DRBMesh>()->getDatas()->setRenderModes(_renderMode);
			entity->getLink().pushAnObject(handle);
		}
	}

	void MeshRenderer::_resetDrawableHandle()
	{
		if (_drawableHandle.invalid() == false)
		{
			for (auto &sm : _drawableHandle.getHandles())
				entity->getLink().popAnObject(sm);
			auto manager = entity->getScene()->getInstance<GraphicElementManager>();
			manager->removeMesh(_drawableHandle);
		}
	}


	bool MeshRenderer::reload_material() {
		if (_material) {
			auto scene = entity->getScene();
			if (entity->getScene()->getInstance<AGE::AssetsManager>()->material_was_reloaded(_materialPath)) {
				if (_mesh && _material)
				{
					setMeshAndMaterial(_mesh, _material);
					return true;
				}
			}
		}
		return false;
	}

	void MeshRenderer::postUnserialization()
	{
		auto scene = entity->getScene();

		auto meshptr = scene->getInstance<AGE::AssetsManager>()->getMesh(_meshPath);
		if (!_meshPath.empty() && (!meshptr || !meshptr->isValid()))
		{
			scene->getInstance<AGE::AssetsManager>()->pushNewCallback(_meshPath, scene,
				std::function<void()>([=](){
				_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(_meshPath);
				setMeshAndMaterial(_mesh, _material);
			}));
			scene->getInstance<AGE::AssetsManager>()->loadMesh(_meshPath, _meshPath);
		}
		else
		{
			_mesh = meshptr;
		}

		auto materialptr = scene->getInstance<AGE::AssetsManager>()->getMaterial(_materialPath);
		if (!_materialPath.empty() && (!materialptr || !materialptr->isValid()))
		{
			scene->getInstance<AGE::AssetsManager>()->pushNewCallback(_materialPath, scene,
				std::function<void()>([=](){
				_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(_materialPath);
				setMeshAndMaterial(_mesh, _material);
			}));
			scene->getInstance<AGE::AssetsManager>()->loadMaterial(_materialPath, _materialPath);
		}
		else
		{
			_material = materialptr;
		}

		setMeshAndMaterial(_mesh, _material);
	}

#ifdef EDITOR_ENABLED
	bool MeshRenderer::editorUpdate()
	{
		bool modified = false;
		auto scene = entity->getScene();

		if ((*meshPathList)[selectedMeshIndex] != _meshPath)
		{
			std::size_t i = 0;
			for (auto &e : *meshPathList)
			{
				if (e == _meshPath)
				{
					selectedMeshIndex = i;
					modified = true;
					break;
				}
				++i;
			}
		}
		//---
		if ((*materialPathList)[selectedMaterialIndex] != _materialPath)
		{
			std::size_t i = 0;
			for (auto &e : *materialPathList)
			{
				if (e == _materialPath)
				{
					selectedMaterialIndex = i;
					modified = true;
					break;
				}
				++i;
			}
		}

		std::string occlusionButtonStr = "Occluder : ";
		
		if (_renderMode.test(AGE_OCCLUDER))
		{
			occlusionButtonStr += "TOGGLE OFF";
		}
		else
		{
			occlusionButtonStr += "TOGGLE ON";
		}

		if (ImGui::Button(occlusionButtonStr.c_str()))
		{
			_renderMode[AGE_OCCLUDER].flip();
			_updateGeometry();
			modified = true;
		}

		ImGui::PushItemWidth(-1);
		if (ImGui::ListBox("Meshs", (int*)&selectedMeshIndex, &(meshFileList->front()), (int)(meshFileList->size())))
		{
			modified = true;

			selectedMeshName = (*meshFileList)[selectedMeshIndex];
			_meshPath = (*meshPathList)[selectedMeshIndex];

			_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(_meshPath);

			if (!_mesh)
			{
				scene->getInstance<AGE::AssetsManager>()->pushNewCallback(_meshPath, scene,
					std::function<void()>([=](){
					_mesh = scene->getInstance<AGE::AssetsManager>()->getMesh(_meshPath);
					if (_mesh && _material)
					{
						setMeshAndMaterial(_mesh, _material);
					}
				}));
				scene->getInstance<AGE::AssetsManager>()->loadMesh(OldFile(_meshPath), _meshPath);
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
		//---
		ImGui::PushItemWidth(-1);
		if (!materialFileList->empty() && ImGui::ListBox("Material", (int*)&selectedMaterialIndex, &(materialFileList->front()), (int)(materialFileList->size())))
		{
			modified = true;

			selectedMaterialName = (*materialFileList)[selectedMaterialIndex];
			_materialPath = (*materialPathList)[selectedMaterialIndex];

			_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(_materialPath);

			if (!_material)
			{
				scene->getInstance<AGE::AssetsManager>()->pushNewCallback(_materialPath, scene,
					std::function<void()>([=](){
					_material = scene->getInstance<AGE::AssetsManager>()->getMaterial(_materialPath);
					if (_mesh && _material)
					{
						setMeshAndMaterial(_mesh, _material);
					}
				}));
				scene->getInstance<AGE::AssetsManager>()->loadMaterial(OldFile(_materialPath), _materialPath);
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
		//---
		return modified;
	}
#endif
}