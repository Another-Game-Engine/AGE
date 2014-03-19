#pragma once

#include <Systems/System.h>

class FPSSystem : public System
{
public:
	FPSSystem(std::weak_ptr<AScene> scene);
	virtual ~FPSSystem(void);

private:
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual void initialize();
	float _timeCounter = 0.0f;
	std::size_t _frameCounter = 0;
	std::size_t _lastFrame = 0;
};