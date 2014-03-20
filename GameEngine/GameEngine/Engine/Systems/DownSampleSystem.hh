#pragma once
#include <Systems\System.h>
#include <Utils/DrawQuad.hh>

class DownSampleSystem : public System
{
public:
	DownSampleSystem(std::weak_ptr<AScene> scene);
	virtual ~DownSampleSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual void initialize();

private:
	EntityFilter						_cameraFilter;

	// DownSample quad
	DrawQuad						_quad;
};

