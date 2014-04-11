#define GLM_FORCE_INLINE 
#define GLM_FORCE_SSE2 
#define GLM_FORCE_AVX

#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include <Core/Engine.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>

// SCENES
#include "Scenes/BenchmarkScene.hpp"

// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>


int			main(int ac, char **av)
{
	std::shared_ptr<Engine>	e = std::make_shared<Engine>();

	auto config = e->setInstance<ConfigurationManager>(File("MyConfigurationFile.conf"));
	e->setInstance<PubSub::Manager>();
	e->setInstance<SdlContext, IRenderContext>();
	e->setInstance<Input>();
	e->setInstance<Timer>();
	e->setInstance<Renderer>();
	e->setInstance<SceneManager>();

	std::function<void(glm::uvec2 &&)> f = [](glm::uvec2 &&v){std::cout << "modified : " << v.x << std::endl; };
	config->setConfiguration<glm::uvec2>("windowSize", glm::uvec2(800, 600), f);
	config->setValue<glm::uvec2>("windowSize", glm::uvec2(840, 640));

	// init engine
	if (e->init(0, 800, 600, "~AGE~ V0.0 Demo") == false)
		return (EXIT_FAILURE);


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
	e->stop();
	return (EXIT_SUCCESS);
}
