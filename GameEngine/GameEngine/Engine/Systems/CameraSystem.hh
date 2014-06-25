#pragma once

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/MatrixConversion.hpp>
#include <Text/FontManager.hh>

class CameraSystem : public System
{
public:
	CameraSystem(std::weak_ptr<AScene> &&scene);
	virtual ~CameraSystem(){}

	void setRenderDebugMode(bool t);
	bool getRenderDebugMode() const;
	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to);
	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to);
	double getLifeTime() const;

protected:
	bool _renderDebugMethod;
	double	_totalTime;
	EntityFilter _filter;

	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};