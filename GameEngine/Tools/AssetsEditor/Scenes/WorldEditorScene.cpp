#include "WorldEditorScene.hpp"
#include <imgui\imgui.h>
#include <Systems/EntityManager.hpp>
#include <Systems/AssetsAndComponentRelationsSystem.hpp>
#include <Physic/BulletDynamicManager.hpp>
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
#include <Systems/BulletDynamicSystem.hpp>
#include <Components/EntityRepresentation.hpp>

#include <EditorConfiguration.hpp>

namespace AGE
{
	const std::string WorldEditorScene::Name = "WorldEditor";

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
		setInstance<AGE::BulletDynamicManager, AGE::BulletCollisionManager>()->init();
		getInstance<AGE::AssetsManager>()->setAssetsDirectory(WE::EditorConfiguration::GetCookedDirectory());
		getInstance<AGE::BulletCollisionManager>()->setAssetsDirectory(WE::EditorConfiguration::GetCookedDirectory());

		addSystem<WE::AssetsAndComponentRelationsSystem>(0);
		addSystem<WE::EntityManager>(1);
		addSystem<FreeFlyCamera>(2);
		//addSystem<BulletDynamicSystem>(3);

		return true;
	}

	bool WorldEditorScene::_userUpdateBegin(float time)
	{
		ImGui::BeginChild("World editor", ImVec2(0, 0), true);
		return true;
	}

	bool WorldEditorScene::_userUpdateEnd(float time)
	{
		ImGui::EndChild();
		ImGui::End();

		// TODO
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::MainToPrepare::PrepareDrawLists>();
		// TODO
		AGE::GetPrepareThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::RenderDrawLists>();

		return true;
	}
}