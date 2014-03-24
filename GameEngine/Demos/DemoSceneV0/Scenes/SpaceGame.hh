#pragma once
#include <Core\AScene.hh>

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

