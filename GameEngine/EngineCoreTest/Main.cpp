#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>
#include <stdint.h>

#include <stdlib.h>
#include <Core/Engine.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Utils/PubSub.hpp>

#include <OpenGL/GeometryManager.hh>
#include <OpenGL/ShadingManager.hh>
// SCENES
#include "Scenes/BenchmarkScene.hpp"

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Timer.hh>
#include <Utils/PubSub.hpp>

//CONFIGS
#include <CONFIGS.hpp>

bool loadShaders(std::shared_ptr<Engine> e)
{
	std::string	perModelVars[] =
	{
		"model"
	};

	std::string	perFrameVars[] =
	{
		"projection",
		"view",
		"time",
		"pointLightNbr",
		"spotLightNbr"
	};

	std::string	materialBasic[] =
	{
		"ambient",
		"diffuse",
		"specular",
		"transmittance",
		"emission",
		"shininess"
	};

	std::string	perLightVars[] =
	{
		"lightVP"
	};

	auto s = e->getInstance<Renderer>()->addShader("MaterialBasic",
		"../../Shaders/MaterialBasic.vp",
		"../../Shaders/MaterialBasic.fp");

	auto shadowDepth = e->getInstance<Renderer>()->addShader("ShadowDepth", "../../Shaders/ShadowMapping.vp", "../../Shaders/ShadowMapping.fp");

	e->getInstance<Renderer>()->addUniform("MaterialBasic")
		->init(s, "MaterialBasic", materialBasic);
	e->getInstance<Renderer>()->addUniform("PerFrame")
		->init(s, "PerFrame", perFrameVars);
	e->getInstance<Renderer>()->addUniform("PerModel")
		->init(s, "PerModel", perModelVars);
	e->getInstance<Renderer>()->addUniform("PerLight")
		->init(shadowDepth, "PerLight", perLightVars);

	e->getInstance<Renderer>()->addShader("2DText",
		"../../Shaders/2DText.vp",
		"../../Shaders/2DText.fp");

	e->getInstance<Renderer>()->addShader("SpriteBasic",
		"../../Shaders/SpriteBasic.vp",
		"../../Shaders/SpriteBasic.fp");

	e->getInstance<Renderer>()->addShader("basicLight", "../../Shaders/light.vp", "../../Shaders/light.fp");
	e->getInstance<Renderer>()->addShader("depthOnly", "../../Shaders/depthOnly.vp", "../../Shaders/depthOnly.fp");
	e->getInstance<Renderer>()->bindShaderToUniform("ShadowDepth", "PerModel", "PerModel");
	e->getInstance<Renderer>()->bindShaderToUniform("ShadowDepth", "PerLight", "PerLight");

	e->getInstance<Renderer>()->bindShaderToUniform("depthOnly", "PerFrame", "PerFrame");
	e->getInstance<Renderer>()->bindShaderToUniform("depthOnly", "PerModel", "PerModel");

	e->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerFrame", "PerFrame");
	e->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "PerModel", "PerModel");
	e->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "MaterialBasic", "MaterialBasic");

	std::string	vars[] =
	{
		"projection",
		"view"
	};

	auto sky = e->getInstance<Renderer>()->addShader("cubemapShader", "../../Shaders/cubemap.vp", "../../Shaders/cubemap.fp");

	e->getInstance<Renderer>()->addUniform("cameraUniform")
		->init(sky, "cameraUniform", vars);

	e->getInstance<Renderer>()->bindShaderToUniform("cubemapShader", "cameraUniform", "cameraUniform");

	//NEW VERSION OF SHADING
	// create the shader
	auto &m = e->getInstance<gl::ShadingManager>();
	auto &basic = m->addShader("../../Shaders/MaterialBasic.vp", "../../Shaders/MaterialBasic.fp"); // 0
	auto &shadow = m->addShader("../../Shaders/ShadowMapping.vp", "../../Shaders/ShadowMapping.fp"); // 1
	auto &text2d = m->addShader("../../Shaders/2DText.vp", "../../Shaders/2DText.fp"); // 2
	auto &sprite = m->addShader("../../Shaders/SpriteBasic.vp", "../../Shaders/SpriteBasic.fp"); // 3
	auto &depthonly = m->addShader("../../Shaders/depthOnly.vp", "../../Shaders/depthOnly.fp"); // 4
	auto &cubemap = m->addShader("../../Shaders/cubemap.vp", "../../Shaders/cubemap.fp"); // 5

	// create the uniform block
	size_t sizeBlock[6];
	gl::set_tab_sizetype<glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, float>(sizeBlock);
	auto &ubMaterialBasic = m->addUniformBlock(6, sizeBlock);
	gl::set_tab_sizetype<glm::mat4, glm::mat4, float, unsigned int, unsigned int>(sizeBlock);
	auto &ubPerFrame = m->addUniformBlock(5, sizeBlock);
	auto &ubPerModel = m->addUniformBlock(1, sizeBlock);
	auto &ubPerLighting = m->addUniformBlock(1, sizeBlock);
	gl::set_tab_sizetype<glm::mat4, glm::mat4>(sizeBlock);
	auto &ubCameraUniform = m->addUniformBlock(2, sizeBlock);

	// create and set the interface
	auto &imbmb = m->addShaderInterfaceBlock(basic, "MaterialBasic", ubMaterialBasic);
	auto &imbpf = m->addShaderInterfaceBlock(basic, "PerFrame", ubPerFrame);
	auto &imbpm = m->addShaderInterfaceBlock(basic, "PerModel", ubPerModel);
	auto &ismpm = m->addShaderInterfaceBlock(shadow, "PerModel", ubPerModel);
	auto &ismpl = m->addShaderInterfaceBlock(shadow, "PerLight", ubPerLighting);
	auto &idopm = m->addShaderInterfaceBlock(depthonly, "PerModel", ubPerModel);
	auto &idopf = m->addShaderInterfaceBlock(depthonly, "PerFrame", ubPerFrame);
	auto &icu = m->addShaderInterfaceBlock(cubemap, "cameraUniform", ubCameraUniform);
	auto &us2dtt = m->addShaderSampler(text2d, "fTexture0");
	auto &u2dtp = m->addShaderUniform(text2d, "projection");
	auto &u2dtt = m->addShaderUniform(text2d, "transformation");
	auto &u2dtc = m->addShaderUniform(text2d, "color");
	auto &ussbt = m->addShaderSampler(sprite, "fTexture0");
	auto &usbp = m->addShaderUniform(sprite, "projection");
	auto &usbv = m->addShaderUniform(sprite, "view");
	auto &usbt = m->addShaderUniform(sprite, "transformation");
	auto &usbc = m->addShaderUniform(sprite, "color");

	// set uniform and sampler


	//
	//gl::Key<gl::Shader> test_pipeline_1 = m->addShader("../../test_pipeline_1.vp", "../../test_pipeline_1.fp");
	//auto &pm = m->addShaderUniform(test_pipeline_1, "projection_matrix", glm::perspective<float>(60.f, 600.f / 800.f, 1.0f, 100.0f));
	//auto &mm = m->addShaderUniform(test_pipeline_1, "modelview_matrix", glm::mat4(1.f));
	//size_t sizeData[2] = {sizeof(glm::vec4), sizeof(glm::mat4)};
	//auto &ub = m->addUniformBlock(2, sizeData);
	//auto &ib = m->addShaderInterfaceBlock(test_pipeline_1, "test", ub);
	//m->setUniformBlock(ub, 0, glm::vec4(1.0, 0.0, 1.0, 2.0));
	//m->setUniformBlock(ub, 1, glm::mat4(1.0));
	return true;
}

bool loadAssets(std::shared_ptr<Engine> e)
{
#ifdef RENDERING_ACTIVATED
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__cube.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__ball.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
#endif
#ifdef COMPLEX_MESH
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Space.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__cube.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__ball.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));
	e->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__dragon.cpd"));
#endif

	return true;
}

int			main(int ac, char **av)
{
	std::shared_ptr<Engine>	e = std::make_shared<Engine>();

	// Set Configurations
	auto config = e->setInstance<ConfigurationManager>(File("MyConfigurationFile.conf"));

	e->setInstance<PubSub::Manager>();
	e->setInstance<SdlContext, IRenderContext>();
	e->setInstance<Input>();
	e->setInstance<Timer>();
	e->setInstance<Renderer>();
	e->setInstance<SceneManager>();
	e->setInstance<AssetsManager>()->init();

#ifdef PHYSIC_SIMULATION
	e->setInstance<BulletDynamicManager, BulletCollisionManager>()->init();
#endif


	// init engine
	if (e->init(0, 800, 600, "~AGE~ V0.0 Demo") == false)
		return (EXIT_FAILURE);

	// Set default window size
	// If config file has different value, it'll be changed automaticaly
	config->setConfiguration<glm::uvec2>("windowSize", glm::uvec2(800, 600), [&e](const glm::uvec2 &v)
	{
		e->getInstance<IRenderContext>()->setScreenSize(std::move(v));
	});

	config->loadFile();

#ifdef RENDERING_ACTIVATED

	auto &m = e->setInstance<gl::GeometryManager>();
	// create pool
	m->addIndexPool();
	m->addVertexPool();
	GLenum typeComponent[2] = {GL_FLOAT, GL_FLOAT};
	uint8_t sizeTypeComponent[2] = { sizeof(float), sizeof(float) };
	uint8_t nbrComponent[2] = {2, 2};
	m->addVertexPool(2, typeComponent, sizeTypeComponent, nbrComponent);
	
	// attach pool which be create
	m->attachIndexPoolToVertexPool(m->getVertexPool(0), m->getIndexPool(0));
	m->attachIndexPoolToVertexPool(m->getVertexPool(1), m->getIndexPool(0));

	e->setInstance<gl::ShadingManager>();

	if (!loadShaders(e))
		return EXIT_FAILURE;
	if (!loadAssets(e))
		return EXIT_FAILURE;
#endif

	// add main scene
	e->getInstance<SceneManager>()->addScene(std::make_shared<BenchmarkScene>(e), "BenchmarkScene");

	// bind scene
	if (!e->getInstance<SceneManager>()->initScene("BenchmarkScene"))
		return (EXIT_FAILURE);

	e->getInstance<SceneManager>()->enableScene("BenchmarkScene", 100);

	// launch engine
	if (e->start() == false)
		return (EXIT_FAILURE);
	while (e->update())
		;
	config->saveToFile();
	e->stop();
	return (EXIT_SUCCESS);
}
