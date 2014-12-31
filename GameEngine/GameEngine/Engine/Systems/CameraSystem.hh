#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Components/CameraComponent.hpp>
#include <Core/PreparableObject.hh>

//namespace gl
//{
//	class Shader;
//	struct Uniform;
//	class UniformBlock;
//	class RenderPass;
//	class RenderOnScreen;
//	class Pipeline;
//}

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
	EntityFilter _drawable;
	EntityFilter _camera;
	
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};