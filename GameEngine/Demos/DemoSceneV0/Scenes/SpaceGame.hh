#pragma once
#include <Core\AScene.hh>
#include <Utils/PubSub.hpp>

class SpaceGame : public AScene
{
public:
	SpaceGame(std::weak_ptr<Engine> engine);
	virtual ~SpaceGame();

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
private:
	std::unique_ptr<PubSub> _globalPubSub;
};

