#ifndef ASCENE_HH_
#define ASCENE_HH_

#include "ACamera.hh"
#include "Entities/AEntity.hh"

#include <list>

class AScene
{
private:
	SmartPointer<AEntity>								_root;
	std::list<SmartPointer<Components::AComponent> >	_components;
	ACamera 											*_camera;

	void 				recomputePositions(SmartPointer<AEntity> &father,
											bool hasMoved);

public:
	AScene();
	virtual ~AScene();

	SmartPointer<AEntity>	const	&getRoot();
	void 							setCamera(ACamera *camera);
	ACamera 						*getCamera() const;

	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate() = 0;
	void 					update();
	void					recomputePositions();
};

#endif