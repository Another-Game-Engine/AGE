#include "AssetsAndComponentRelationsSystem.hpp"
#include <imgui/imgui.h>
#include <Components/RigidBody.hpp>
#include <Components/MeshRenderer.hh>
#include <Scenes/AssetsEditorScene.hpp>
#include <AssetFiles/RawFile.hpp>

namespace AGE
{
	namespace WE
	{
		AssetsAndComponentRelationsSystem::AssetsAndComponentRelationsSystem(std::weak_ptr<AScene> &&scene)
			: System(std::move(scene))
			, _rigidBodies(std::move(scene))
			, _meshRenderer(std::move(scene))
		{
			_name = "we_assets_and_component_relations";
		}
		AssetsAndComponentRelationsSystem::~AssetsAndComponentRelationsSystem(){}

		void AssetsAndComponentRelationsSystem::updateBegin(double time)
		{}

		void AssetsAndComponentRelationsSystem::updateEnd(double time)
		{}

		void AssetsAndComponentRelationsSystem::mainUpdate(double time)
		{
			for (auto e : _meshRenderer.getCollection())
			{
				e.getComponent<MeshRenderer>()->meshFileList = &AssetsEditorScene::getCookedMeshsList();
				e.getComponent<MeshRenderer>()->meshPathList = &AssetsEditorScene::getCookedMeshsListFullPath();
			}
		}

		bool AssetsAndComponentRelationsSystem::initialize()
		{
			_rigidBodies.requireComponent<AGE::RigidBody>();
			_meshRenderer.requireComponent<AGE::MeshRenderer>();
			return true;
		}
	}
}