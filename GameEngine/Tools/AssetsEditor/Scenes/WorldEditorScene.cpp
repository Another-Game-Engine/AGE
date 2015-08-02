#include "WorldEditorScene.hpp"
#include <imgui\imgui.h>
#include <Systems/EntityManager.hpp>
#include <Systems/AssetsAndComponentRelationsSystem.hpp>
#include <Systems/PhysicsSystem.hpp>
#include <AssetManagement/AssetManager.hh>

#include <Components/CameraComponent.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/Commands/MainToPrepareCommands.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Components/FreeFlyComponent.hh>
#include <Systems/FreeFlyCamera.hh>
#include <Components/EntityRepresentation.hpp>
#include <Managers/ArchetypesEditorManager.hpp>
#include <EditorConfiguration.hpp>

namespace AGE
{
	const std::string WorldEditorScene::Name = "World Editor";

	WorldEditorScene::WorldEditorScene(AGE::Engine *engine)
		: AScene(engine)
	{
	}

	WorldEditorScene::~WorldEditorScene(void)
	{
	}

	bool WorldEditorScene::_userStart()
	{
		WE::EditorConfiguration::RefreshScenesDirectoryListing();
		getInstance<AGE::AssetsManager>()->setAssetsDirectory(WE::EditorConfiguration::GetCookedDirectory());

		addSystem<WE::AssetsAndComponentRelationsSystem>(0);
		addSystem<WE::EntityManager>(1);
		addSystem<FreeFlyCamera>(2);
		addSystem<PhysicsSystem>(3, Physics::EngineType::Bullet, getInstance<AGE::AssetsManager>());
		getInstance<Physics::PhysicsInterface>()->getWorld()->setGravity(0, 0, 0);
		return true;
	}

	bool WorldEditorScene::_userUpdateBegin(float time)
	{
		return true;
	}

	bool WorldEditorScene::_userUpdateEnd(float time)
	{
		if (_displayWindow)
		{
			getInstance<AGE::WE::ArchetypeEditorManager>()->update(this);
		}
		// TODO
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
		// TODO
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::RenderDrawLists>();
		return true;
	}

	void WorldEditorScene::updateMenu()
	{
		if (ImGui::BeginMenu("Scene"))
		{
			getSystem<WE::EntityManager>()->updateMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Archetypes"))
		{
			getInstance<AGE::WE::ArchetypeEditorManager>()->updateMenu();
			ImGui::EndMenu();
		}
	}
}