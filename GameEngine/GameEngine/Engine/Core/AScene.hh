#ifndef ASCENE_HH_
#define ASCENE_HH_

#include "ACamera.hh"
#include "Entities/Entity.hh"

#include <list>
#include <queue>

class AScene
{
private:
	SmartPointer<Entity>								_root;
	ACamera 											*_camera;
	std::list<SmartPointer<Entity> >                    _collection;
	std::queue<SmartPointer<Entity> >                   _pool;

	void 				recomputePositions(SmartPointer<Entity> &father,
											bool hasMoved);

public:
	AScene();
	virtual ~AScene();

	SmartPointer<Entity>	const	&getRoot();
	void 							setCamera(ACamera *camera);
	ACamera 						*getCamera() const;
	SmartPointer<Entity>            &createEntity();

	virtual bool 			userStart() = 0;
	virtual bool 			userUpdate() = 0;
	void 					update();
};

#endif