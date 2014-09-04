#pragma once

#include "Systems/System.h"

class LifetimeSystem : public System
{
public:
	LifetimeSystem(std::weak_ptr<AScene> &&scene);
	virtual ~LifetimeSystem();
private:
	EntityFilter _filter;

	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};