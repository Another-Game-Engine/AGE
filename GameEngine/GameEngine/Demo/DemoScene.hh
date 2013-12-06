#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"

class DemoScene : public AScene
{
public:
	DemoScene(Engine &engine);
	virtual ~DemoScene(void);

	SmartPointer<Entity>	DemoScene::createPlanet(float rotSpeed, float orbitSpeed, glm::vec3 &pos, glm::vec3 &scale,
													std::string const &shader,
													std::string const &tex1, std::string const &tex2 = "", std::string const &tex3 = "",
													std::string const &tex4 = "");

	virtual bool 			userStart();
	virtual bool 			userUpdate();
};

#endif