#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"
#include "Core/Engine.hh"
#include <Utils/PubSub.hpp>

class SponzaScene : public AScene
{
public:
	SponzaScene(std::weak_ptr<Engine> engine);
	virtual ~SponzaScene(void);

	Entity  SponzaScene::createSphere(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);
	Entity  SponzaScene::createCube(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);
	Entity  SponzaScene::createMonkey(glm::vec3 &pos, glm::vec3 &scale, std::string const &tex, float mass);

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
private:
	std::unique_ptr<PubSub> _globalPubSub;
};

#endif