#ifndef		BASICCAM_HH_
#define		BASICCAM_HH_

#include "Core\ACamera.hh"

#include "Entities/Entity.hh"

class BasicCam : public ACamera
{
public:
	BasicCam(SmartPointer<Entity> const &toLook);
	virtual ~BasicCam(void);
private:
	glm::vec3				_pos;
	SmartPointer<Entity>	_toLook;

	virtual void      customUpdate();
};

#endif