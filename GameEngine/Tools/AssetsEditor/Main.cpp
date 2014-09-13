#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

#include <Utils/Containers/Vector.hpp>
#include <map>
#include <chrono>

#include <Core/Engine.hh>
#include <Core/SceneManager.hh>

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Timer.hh>
#include <Utils/PubSub.hpp>
#include <Render/GeometryManager.hh>
#include <Utils/PerformanceDebugger.hh>
#include <Core/AssetsManager.hpp>

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


int			main(int ac, char **av)
{
	std::shared_ptr<Engine>	e = std::make_shared<Engine>();

	// Set Configurations
	auto config = e->setInstance<ConfigurationManager>(File("MyConfigurationFile.conf"));

	e->setInstance<PubSub::Manager>();
	e->setInstance<SdlContext, IRenderContext>();
	e->setInstance<Input>();
	e->setInstance<Timer>();
	e->setInstance<SceneManager>();
	e->setInstance<AGE::AssetsManager>();
	e->setInstance<PerformanceDebugger>();
//	auto geometryManager = e->setInstance<gl::GeometryManager>();

	// init engine
	if (e->init(0, 800, 600, "~AGE~ V0.0 Demo") == false)
		return (EXIT_FAILURE);

	// Set default window size
	// If config file has different value, it'll be changed automaticaly
	config->setConfiguration<glm::uvec2>("windowSize", glm::uvec2(800, 600), [&e](const glm::uvec2 &v)
	{
		//e->getInstance<IRenderContext>()->setScreenSize(std::move(v));
	});

	config->loadFile();

	// launch engine
	if (e->start() == false)
		return (EXIT_FAILURE);

	//////////////////////////
	/////
	//

	/////Convert fbx to AGE data structure

	bool isSkeleton = false;
	bool isAnimations = false;
	bool isMesh = false;
	bool isMaterial = false;
	bool isTexture = false;
	bool convert = true;
	if (convert)
	{
		//AGE::AssetDataSet dataSet;
		//dataSet.filePath = File("catwoman/atk close front 6.fbx");
		//dataSet.skeletonName = "catwoman";
		//dataSet.skinName = "catwoman";
		//dataSet.materialName = "catwoman";
		//dataSet.animationName = "catwoman-roulade";		

		//dataSet.serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
		//dataSet.rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/Raw");

		//isMaterial = AGE::MaterialLoader::load(dataSet);
		//isTexture = AGE::ImageLoader::load(dataSet);
		//isSkeleton = AGE::SkeletonLoader::load(dataSet);
		//isAnimations = AGE::AnimationsLoader::load(dataSet);
		//isMesh = AGE::MeshLoader::load(dataSet);

		//AGE::MaterialLoader::save(dataSet);
		//AGE::ImageLoader::save(dataSet);
		//AGE::SkeletonLoader::save(dataSet);
		//AGE::AnimationsLoader::save(dataSet);
		//AGE::MeshLoader::save(dataSet);
	}
	{
		AGE::AssetDataSet dataSet;
		dataSet.filePath = File("ball/ball.obj");
		dataSet.skinName = "ball";
		dataSet.materialName = "ball";

		dataSet.serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
		dataSet.rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

		AGE::MaterialLoader::load(dataSet);
		AGE::ImageLoader::load(dataSet);
		AGE::MeshLoader::load(dataSet);

		AGE::MaterialLoader::save(dataSet);
		AGE::ImageLoader::save(dataSet);
		AGE::MeshLoader::save(dataSet);
	}
	{
		AGE::AssetDataSet dataSet;
		dataSet.filePath = File("cube/cube.obj");
		dataSet.skinName = "cube";
		dataSet.materialName = "cube";

		dataSet.serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
		dataSet.rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

		AGE::MaterialLoader::load(dataSet);
		AGE::ImageLoader::load(dataSet);
		AGE::MeshLoader::load(dataSet);

		AGE::MaterialLoader::save(dataSet);
		AGE::ImageLoader::save(dataSet);
		AGE::MeshLoader::save(dataSet);
	}
//
//	//Load assets from serialized file
//	auto assetsManager = e->getInstance<AGE::AssetsManager>();
//
//	auto catwomanMesh = assetsManager->loadMesh("../../Assets/NewSerialized/catwoman/catwoman.sage"); // load mesh
//	auto catwomanSkeleton = assetsManager->loadSkeleton("../../Assets/NewSerialized/catwoman/catwoman.skage"); // load skeleton
//	std::shared_ptr<AGE::Animation> catwomanRoulade = nullptr;
//	if (isAnimations)
//		catwomanRoulade = assetsManager->loadAnimation("../../Assets/NewSerialized/catwoman/catwoman-roulade.aage"); // load animation
//
// 	AGE::AnimationInstance catwomanAnimationInstance(catwomanSkeleton, catwomanRoulade);
//
//	//
//	/////
//	//////////////////////////
//
//
//
//	auto shader = e->getInstance<Renderer>()->addShader("basic",
//		"./basic.vp",
//		"./basic.fp");
//	if (shader->getId() < 0)
//		return EXIT_FAILURE;
//	shader->use();
//
//
//
//	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//	glm::mat4 Projection = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
//	// Camera matrix
//	float lookAtX = 0;
//	float lookAtY = 0;
//	// Model matrix : an identity matrix (model will be at the origin)
//	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
//	Model = glm::scale(Model, glm::vec3(0.2f));
//
//
//	// On enable la depth car par defaut elle est pas active
//	// Conseil : la mettre dans son game engine
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
//	// la depth de clear par defaut sera 1
//	glClearDepth(1.0f);
//	// la couleur de clear par defaut sera du noir
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//
//	do
//	{
//		auto time = e->getInstance<Timer>()->getElapsed();
//		static float totalTime = 0.0f;
//		totalTime += time;
//
//		glm::vec4 color;
//		shader->use();
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		color = glm::vec4(1, 0, 1, 1);
//
//		if (e->getInstance<Input>()->getKey(SDLK_w))
//			lookAtY += 1;
//		if (e->getInstance<Input>()->getKey(SDLK_s))
//			lookAtY -= 1;
//		if (e->getInstance<Input>()->getKey(SDLK_a))
//			lookAtX -= 1;
//		if (e->getInstance<Input>()->getKey(SDLK_d))
//			lookAtX += 1;
//
//		glm::mat4 View = glm::lookAt(
//			glm::vec3(lookAtX, lookAtY, 150), // Camera is at (4,3,3), in World Space
//			glm::vec3(lookAtX, lookAtY, 0), // and looks at the origin
//			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
//			);
//
//		auto colorId = glGetUniformLocation(shader->getId(), "color");
//		auto modelId = glGetUniformLocation(shader->getId(), "model");
//		auto viewId = glGetUniformLocation(shader->getId(), "view");
//		auto projectionId = glGetUniformLocation(shader->getId(), "projection");
//
//		glUniform4fv(colorId, 1, &color[0]);
//		glUniformMatrix4fv(modelId, 1, GL_FALSE, &Model[0][0]);
//		glUniformMatrix4fv(viewId, 1, GL_FALSE, &View[0][0]);
//		glUniformMatrix4fv(projectionId, 1, GL_FALSE, &Projection[0][0]);
//
////		for (auto i = 0; i < 500; ++i)
//			catwomanAnimationInstance.update(totalTime);
//		catwomanSkeleton->updateSkinning();
//		glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "bones"), catwomanAnimationInstance.transformations.size(), GL_FALSE, glm::value_ptr(catwomanAnimationInstance.transformations[0]));
//
//		for (unsigned int i = 0; i < catwomanMesh->subMeshs.size(); ++i)
//		{
//			geometryManager->draw(GL_TRIANGLES, catwomanMesh->subMeshs[i].indices, catwomanMesh->subMeshs[i].vertices);
//		}
//	} while (e->update());
//
//	config->saveToFile();
//	e->stop();
	return EXIT_SUCCESS;
}