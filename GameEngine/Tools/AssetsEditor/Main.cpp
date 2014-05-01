#include <stdlib.h>
#include <Core/Engine.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Utils/PubSub.hpp>
#include <OpenGL/VertexManager.hh>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <chrono>

#include <AGE_FBX/Mesh.hpp>


// DEPENDENCIES
#include <Context/SdlContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <GameplayConvertor.hpp>

// FBX
#include "fbx/FBXSceneEncoder.h"

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
	e->setInstance<AGE::GameplayConvertor>();

	// init engine
	if (e->init(0, 800, 600, "~AGE~ V0.0 Demo") == false)
		return (EXIT_FAILURE);

	// Set default window size
	// If config file has different value, it'll be changed automaticaly
	//config->setConfiguration<glm::uvec2>("windowSize", glm::uvec2(1920, 1080), [&e](const glm::uvec2 &v)
	//{
	//	e->getInstance<IRenderContext>()->setScreenSize(std::move(v));
	//});

	config->loadFile();

	std::array<Attribute, 8> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 4)
 		, Attribute(GL_FLOAT, sizeof(float), 4)
 		, Attribute(GL_FLOAT, sizeof(float), 4)
 		, Attribute(GL_FLOAT, sizeof(float), 4)
 		, Attribute(GL_FLOAT, sizeof(float), 2)
 		, Attribute(GL_FLOAT, sizeof(float), 4)
 		, Attribute(GL_FLOAT, sizeof(float), 4)
 		, Attribute(GL_FLOAT, sizeof(float), 4)
	};

	e->setInstance<VertexManager<8>>(param)->init();

	// launch engine
	if (e->start() == false)
		return (EXIT_FAILURE);


	FBXSceneEncoder fbxEncoder;
	EncoderArguments args((size_t)(ac), (const char **)(av));
	fbxEncoder.write("../../Assets/catwoman/atk close front 6.fbx", args);

	auto file = fbxEncoder.getGameplayFile();
	gameplay::Object* object = file.getObjects().front();

	for (auto n = file.getNodes().begin(); n != std::end(file.getNodes()); ++n)
	{
		e->getInstance<AGE::GameplayConvertor>()->importNode(*n);
	}


	auto s = e->getInstance<Renderer>()->addShader("basic",
		"./basic.vp",
		"./basic.fp");
	s->use();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(10, 5, 100), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	Model = glm::scale(Model, glm::vec3(0.3f));
	// Our ModelViewProjection : multiplication of our 3 matrices


	// On enable la depth car par defaut elle est pas active
	// Conseil : la mettre dans son game engine
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	// la depth de clear par defaut sera 1
	glClearDepth(1.0f);
	// la couleur de clear par defaut sera du noir
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	auto gameplayconvertor = e->getInstance<AGE::GameplayConvertor>();

	do
	{
		glm::vec4 color;
		s->use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Model = glm::rotate(Model, 10.0f * (float)e->getInstance<Timer>()->getElapsed(), glm::vec3(0, 1, 0));
		color = glm::vec4(1, 0, 1, 1);
		glUniform4fv(glGetUniformLocation(s->getId(), "color"), 1, &color[0]);
		glUniformMatrix4fv(glGetUniformLocation(s->getId(), "model"), 1, GL_FALSE, &Model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(s->getId(), "view"), 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(s->getId(), "projection"), 1, GL_FALSE, &Projection[0][0]);
		gameplayconvertor->mesh->_buffer.draw(GL_TRIANGLES);
	} while (e->update());
	config->saveToFile();
	e->stop();
	return (EXIT_SUCCESS);
}