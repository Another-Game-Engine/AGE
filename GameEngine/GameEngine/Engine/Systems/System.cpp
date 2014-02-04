#include <Systems/System.h>
#include <Core/AScene.hh>

System::System(AScene *scene)
: PubSub(scene->getInstance<PubSub::Manager>())
, _scene(scene)
{}

System::~System()
{
}


void System::update(double time)
{
	updateBegin(time);
	mainUpdate(time);
	updateEnd(time);
}

void System::init()
{
	initialize();
}