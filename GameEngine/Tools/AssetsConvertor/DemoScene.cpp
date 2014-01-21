#include <glm/gtc/matrix_transform.hpp>
#include "Core/Engine.hh"
#include "Core/Renderer.hh"
#include "DemoScene.hh"

#include "ResourceManager/SharedMesh.hh"
#include "ResourceManager/Texture.hh"
#include "ResourceManager/CubeMap.hh"
#include "ResourceManager/ResourceManager.hh"

#include <Managers/AssetsConvertorManager.hh>

#include <Components/RotationForce.hh>
#include <Components/MaterialComponent.h>
#include <Components/CameraComponent.hh>

#include <OpenGL/ComputeShader.hh>
#include <OpenGL/Attribute.hh>
#include <OpenGL/include/SDL/SDL_opengl.h>

#include <Systems/RotationForceSystem.hpp>
#include <Systems/GraphNodeSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/MeshRenderSystem.h>

#include <Core/Engine.hh>

#include <SDL\SDL.h>

DemoScene::DemoScene(Engine &engine) : AScene(engine)
{
}

DemoScene::~DemoScene(void)
{
}

bool 			DemoScene::userStart()
{	
	std::srand(0);

	// System Tests
	//
	//
	addSystem<MeshRendererSystem>(0);
	addSystem<GraphNodeSystem>(0); // UPDATE ENTITIES TRANSFORMATION
	addSystem<CameraSystem>(200); // UPDATE CAMERA AND RENDER TO SCREEN


	// end System Test
	{
		auto &convertor = _engine.getInstance<AssetsConvertorManager>();
		auto success = convertor.load("./Assets/cube/cube.obj", "cube");
		convertor.serializeData();
	}
	{
		//auto &convertor = _engine.getInstance<AssetsConvertorManager>();
		//auto success = convertor.load("./Assets/crytek-sponza/sponza.obj", "sponza");
	}



	return (true);
}

bool 			DemoScene::userUpdate(double time)
{
	if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
		_engine.getInstance<Input>().getInput(SDL_QUIT))
		return (false);
	return (true);
}
