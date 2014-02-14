#pragma once

#include "Core/AScene.hh"
#include "Core/Engine.hh"

class SolarSystemDemoScene : public AScene
{
public:
	SolarSystemDemoScene(Engine &engine);
	virtual ~SolarSystemDemoScene(void);

	Entity	SolarSystemDemoScene::createPlanet(float rotSpeed, float orbitSpeed, glm::vec3 &pos, glm::vec3 &scale,
													std::string const &shader,
													std::string const &tex1, std::string const &tex2 = "NULL", std::string const &tex3 = "NULL",
													std::string const &tex4 = "NULL");

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
};