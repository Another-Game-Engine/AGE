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
			auto &rawList = AssetsEditorScene::getRawList();
			rawList.list();

			_assetRenderMeshList.clear();
			rawList.update(std::function<void(AE::RawFile*)>([&](AE::RawFile* file) {
				_assetRenderMeshList.push_back(file->getFileName().c_str());
			}));

			for (auto e : _meshRenderer.getCollection())
			{
				e.getComponent<MeshRenderer>()->meshFileList = &_assetRenderMeshList;
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