#ifndef   __CAMERA_SYSTEM_HPP__
# define  __CAMERA_SYSTEM_HPP__

#include <memory>
#include <Systems/System.h>
#include <glm/glm.hpp>
#include <Core/EntityFilter.hpp>

class AScene;

class CameraSystem : public System
{
public:
	CameraSystem(std::weak_ptr<AScene> &&scene);

	virtual ~CameraSystem();

	void setRenderDebugMode(bool t);

	bool getRenderDebugMode() const;

	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to);

	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to);

	double		getLifeTime() const;

protected:
	EntityFilter _filter;

	bool _renderDebugMethod;
	double	_totalTime;
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};

#endif    //__CAMERA_SYSTEM_HPP__