#include "AssetsAndComponentRelationsSystem.hpp"
#include <imgui/imgui.h>
#include <Components/RigidBody.hpp>
#include <Components/MeshRenderer.hh>
#include <Scenes/AssetsEditorScene.hpp>
#include <AssetFiles/RawFile.hpp>
#include <Components/EntityRepresentation.hpp>

namespace AGE
{
	namespace WE
	{
		AssetsAndComponentRelationsSystem::AssetsAndComponentRelationsSystem(AScene *scene)
			: System(scene)
			, _rigidBodies(std::move(scene))
			, _meshRenderer(std::move(scene))
			, _all(std::move(scene))
		{
			_name = "we_assets_and_component_relations";
		}
		AssetsAndComponentRelationsSystem::~AssetsAndComponentRelationsSystem(){}

		void AssetsAndComponentRelationsSystem::updateBegin(float time)
		{}

		void AssetsAndComponentRelationsSystem::updateEnd(float time)
		{}

		void AssetsAndComponentRelationsSystem::mainUpdate(float time)
		{
			for (auto e : _meshRenderer.getCollection())
			{
				e.getComponent<MeshRenderer>()->meshFileList = &AssetsEditorScene::getCookedMeshsList();
				e.getComponent<MeshRenderer>()->meshPathList = &AssetsEditorScene::getCookedMeshsListFullPath();
				e.getComponent<MeshRenderer>()->materialFileList = &AssetsEditorScene::getCookedMaterialList();
				e.getComponent<MeshRenderer>()->materialPathList = &AssetsEditorScene::getCookedMaterialListFullPath();
			}
			for (auto e : _rigidBodies.getCollection())
			{
				e.getComponent<RigidBody>()->shapeFileList = &AssetsEditorScene::getCookedBulletList();
				e.getComponent<RigidBody>()->shapePathList = &AssetsEditorScene::getCookedBulletListFullPath();
			}
		}

		bool AssetsAndComponentRelationsSystem::initialize()
		{
			_rigidBodies.requireComponent<AGE::RigidBody>();
			_meshRenderer.requireComponent<AGE::MeshRenderer>();

			_all.setOnAdd(std::function<void(Entity e)>([this](Entity en)
			{
				en.addComponent<AGE::WE::EntityRepresentation>(std::string("Entity " + std::to_string(en.getId()) + "\0").c_str());
			}));

			_all.setOnRemove(std::function<void(Entity e)>([this](Entity en)
			{
			}));
			return true;
		}
	}
}