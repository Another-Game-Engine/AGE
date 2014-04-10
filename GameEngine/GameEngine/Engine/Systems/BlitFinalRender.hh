#pragma once
#include <Systems/System.h>
#include <Utils/DrawQuad.hh>

class BlitFinalRender : public System
{
public:
	BlitFinalRender(std::weak_ptr<AScene> &&scene);
	virtual ~BlitFinalRender();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual bool initialize();
private:
	EntityFilter						_cameraFilter;

	// Blit Quad
	DrawQuad						_quad;
};

