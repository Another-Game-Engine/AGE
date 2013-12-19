#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"
#include "Core/Engine.hh"

class DemoScene : public AScene
{
public:
	DemoScene(Engine &engine);
	virtual ~DemoScene(void);

	Handle	DemoScene::createPlanet(float rotSpeed, float orbitSpeed, glm::vec3 &pos, glm::vec3 &scale,
													std::string const &shader,
													std::string const &tex1, std::string const &tex2 = "", std::string const &tex3 = "",
													std::string const &tex4 = "");
	Handle  DemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &material, std::string const &tex, float mass);
	Handle  DemoScene::createPlane(glm::vec3 &pos, glm::vec3 &scale, std::string const &material, std::string const &tex);
	Handle  DemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &material, std::string const &tex, float mass);

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
};

#endif