#ifndef		DEMOSCENE_HH_
#define		DEMOSCENE_HH_

#include "Core\AScene.hh"

class DemoScene : public AScene
{
public:
	DemoScene(void);
	virtual ~DemoScene(void);

	virtual bool 			userStart();
	virtual bool 			userUpdate();
};

#endif