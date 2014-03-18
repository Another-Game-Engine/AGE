#include <Systems/System.h>
#include <Core/AScene.hh>

System::System(std::weak_ptr<AScene> scene)
: PubSub(scene.lock()->getInstance<PubSub::Manager>())
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