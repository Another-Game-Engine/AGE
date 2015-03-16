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

	bool WorldEditorScene::userStart()
	{
		setInstance<AGE::BulletDynamicManager, AGE::BulletCollisionManager>()->init();
		setInstance<AssetsManager>();
		getInstance<AGE::AssetsManager>()->setAssetsDirectory("../../Assets/Serialized/");

		addSystem<WE::AssetsAndComponentRelationsSystem>(0);
		addSystem<WE::EntityManager>(1);
		addSystem<FreeFlyCamera>(2);
		addSystem<BulletDynamicSystem>(3);
		// Add entity used by editor (camera etc here)

		auto camera = createEntity();
		auto cam = camera.addComponent<CameraComponent>();
		camera.getLink().setPosition(glm::vec3(0, 3, 5));
		camera.getLink().setForward(glm::vec3(0, 0, 0));
		cam->addPipeline(RenderType::BASIC);
		camera.addComponent<FreeFlyComponent>();
		camera.getComponent<AGE::WE::EntityRepresentation>()->editorOnly = true;

		return true;
	}

	bool WorldEditorScene::userUpdateBegin(float time)
	{
		ImGui::BeginChild("Assets browser", ImVec2(0, 0), true);
		return true;
	}

	bool WorldEditorScene::userUpdateEnd(float time)
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