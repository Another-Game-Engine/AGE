#include <Utils/Containers/Vector.hpp>
#include <map>

//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>

//SKINNING
#include <AssetManagement/Data/AnimationData.hpp>
#include <AssetManagement/Instance/AnimationInstance.hh>
#include <Skinning/AnimationChannel.hpp>
#include <Skinning/AnimationKey.hpp>
#include <Skinning/Bone.hpp>
#include <Skinning/Skeleton.hpp>

//CORE
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Core/Timer.hh>
#include <AssetManagement/AssetManager.hh>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Core/ConfigurationManager.hpp>

//SCENE
#include <Scenes/AssetsEditorScene.hpp>
#include <Scenes/SceneSelectorScene.hpp>
#include <Scenes/WorldEditorScene.hpp>

//COMPONENT REGISTRAR
#include <EDITOR_COMPONENT_REGISTER.cpp>

#include <Components/CameraComponent.hpp>
#include <Components/FPController.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/Light.hh>
#include <Components/RigidBody.hpp>
#include <Components/SpotLight.hh>
#include <Components/FreeFlyComponent.hh>

//COMPONENTS
#include <Components/EntityRepresentation.hpp>

int			main(int ac, char **av)
{
	AGE::InitAGE();
	auto engine = AGE::CreateEngine();
	engine->displayFps(false);

	engine->launch(std::function<bool()>([&]()
	{
		auto configurationManager = engine->getInstance<AGE::ConfigurationManager>();
		configurationManager->setConfiguration<std::string>(std::string("ShadersPath"), std::string(engine->getApplicationPath() + "/../../Shaders/"));

		AGE::GetThreadManager()->setAsWorker(false, false, false);
		engine->setInstance<AGE::Timer>();
		engine->setInstance<AGE::AssetsManager>();

		AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=](){
			AGE::Imgui::getInstance()->init(engine);
			return true;
		}).get();

		REGISTER_COMPONENT_TYPE(AGE::WE::EntityRepresentation);
		REGISTER_COMPONENT_TYPE(AGE::MeshRenderer);
		REGISTER_COMPONENT_TYPE(AGE::RigidBody);
		REGISTER_COMPONENT_TYPE(AGE::PointLightComponent);
		REGISTER_COMPONENT_TYPE(AGE::CameraComponent);
		REGISTER_COMPONENT_TYPE(AGE::FreeFlyComponent);

		RegisterComponents();

		engine->addScene(std::make_shared<AGE::AssetsEditorScene>(engine), AGE::AssetsEditorScene::Name);
		engine->addScene(std::make_shared<AGE::SceneSelectorScene>(engine), AGE::SceneSelectorScene::Name);
		engine->addScene(std::make_shared<AGE::WorldEditorScene>(engine), AGE::WorldEditorScene::Name);

		if (!engine->initScene(AGE::AssetsEditorScene::Name))
			return false;
		if (!engine->initScene(AGE::SceneSelectorScene::Name))
			return false;
		if (!engine->initScene(AGE::WorldEditorScene::Name))
			return false;
		engine->enableScene(AGE::AssetsEditorScene::Name, 1000);
		engine->enableScene(AGE::SceneSelectorScene::Name, 1);
		return true;
	}));

	return (0);
}