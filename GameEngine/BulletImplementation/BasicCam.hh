#ifndef		BASICCAM_HH_
#define		BASICCAM_HH_

#include "Core\ACamera.hh"

#include "Entities/Entity.hh"
#include <Entities\Handle.hh>

class BasicCam : public ACamera
{
public:
	BasicCam(Engine &engine, Handle &toLook);
	virtual ~BasicCam(void);
private:
	float					_dist;
	int						_lastKey;
	glm::vec3				_pos;
	Handle	_toLook;

	virtual void      customUpdate();
};

#endif