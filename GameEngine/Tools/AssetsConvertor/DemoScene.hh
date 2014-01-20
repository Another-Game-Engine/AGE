#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"
#include "Core/Engine.hh"

class DemoScene : public AScene
{
public:
	DemoScene(Engine &engine);
	virtual ~DemoScene(void);

	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
};

#endif