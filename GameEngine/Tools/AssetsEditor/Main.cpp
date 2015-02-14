#include <Utils/Containers/Vector.hpp>
#include <map>

//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>

//SKINNING
#include <Skinning/Animation.hpp>
#include <Skinning/AnimationChannel.hpp>
#include <Skinning/AnimationInstance.hpp>
#include <Skinning/AnimationKey.hpp>
#include <Skinning/Bone.hpp>
#include <Skinning/Skeleton.hpp>

//CORE
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Core/Timer.hh>
#include <Core/AssetsManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/TaskScheduler.hpp>

//SCENE
#include <Scenes/AssetsEditorScene.hpp>
#include <Scenes/SceneSelectorScene.hpp>
#include <Scenes/WorldEditorScene.hpp>

//COMPONENT REGISTRAR
#include <Components/ComponentRegistrar.hpp>

//COMPONENTS
#include <Components/CameraComponent.hpp>
#include <Components/FPController.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/Light.hh>
#include <Components/EntityRepresentation.hpp>
#include <Components/RigidBody.hpp>
#include <Components/SpotLight.hh>

int			main(int ac, char **av)
{
	AGE::InitAGE();
	auto engine = AGE::CreateEngine();

	engine.lock()->launch(std::function<bool()>([&]()
	{
		AGE::GetThreadManager()->setAsWorker(false, false, false);
		engine.lock().get()->setInstance<AGE::Timer>();
		engine.lock().get()->setInstance<AGE::AssetsManager>();

		AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=](){
			AGE::Imgui::getInstance()->init(engine.lock().get());
			return true;
		}).get();

#ifdef RENDERING_ACTIVATED
		if (!loadAssets(engine.lock().get()))
			return false;
#endif

		REGISTER_COMPONENT_TYPE(AGE::MeshRenderer);
		REGISTER_COMPONENT_TYPE(AGE::RigidBody);
		REGISTER_COMPONENT_TYPE(AGE::PointLightComponent);
		REGISTER_COMPONENT_TYPE(AGE::CameraComponent);

		engine.lock()->addScene(std::make_shared<AGE::AssetsEditorScene>(engine), AGE::AssetsEditorScene::Name);
		engine.lock()->addScene(std::make_shared<AGE::SceneSelectorScene>(engine), AGE::SceneSelectorScene::Name);
		engine.lock()->addScene(std::make_shared<AGE::WorldEditorScene>(engine), AGE::WorldEditorScene::Name);

		if (!engine.lock()->initScene(AGE::AssetsEditorScene::Name))
			return false;
		if (!engine.lock()->initScene(AGE::SceneSelectorScene::Name))
			return false;
		if (!engine.lock()->initScene(AGE::WorldEditorScene::Name))
			return false;
		engine.lock()->enableScene(AGE::AssetsEditorScene::Name, 1000);
		engine.lock()->enableScene(AGE::SceneSelectorScene::Name, 1);
		return true;
	}));

	return (0);
}