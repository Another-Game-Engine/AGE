#ifndef		BASICCAM_HH_
#define		BASICCAM_HH_

#include "Core\ACamera.hh"

class BasicCam : public ACamera
{
public:
	BasicCam(void);
	virtual ~BasicCam(void);

	virtual void      update();
};

#endif