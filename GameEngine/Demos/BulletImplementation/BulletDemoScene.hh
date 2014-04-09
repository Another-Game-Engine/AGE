#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"
#include "Core/Engine.hh"

class BulletDemoScene : public AScene
{
public:
	BulletDemoScene(std::weak_ptr<Engine> &&engine);
	virtual ~BulletDemoScene(void);

	Entity  BulletDemoScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);
	Entity  BulletDemoScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);
	Entity  BulletDemoScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
};

#endif