
#include "AScene.hh"

AScene::AScene() :
	_camera(NULL)
{
}

AScene::~AScene()
{
}

void 							AScene::recomputePositions(SmartPointer<AEntity> &father,
															bool hasMoved)
{
	AEntity::t_sonsList::iterator 	it = father->getSonsBegin();

	while (it != father->getSonsEnd())
	{
		AEntity::t_ComponentsList::iterator		comp = it->second->getComponentsBegin();

		while (comp != it->second->getComponentsEnd())
			comp->second->update(); // update components
		if (father->getFlags() & AEntity::HAS_MOVED)
			hasMoved = true;
		if (hasMoved)
			it->second->computeGlobalTransform(father->getGlobalTransform());
		recomputePositions(it->second, hasMoved);
		++it;
	}
}

SmartPointer<AEntity>	const	&AScene::getRoot()
{
	return (_root);
}

void 							AScene::setCamera(ACamera *camera)
{
	_camera = camera;
}

ACamera 						*AScene::getCamera() const
{
	return (_camera);
}

void 							AScene::update()
{
	recomputePositions(_root, false);
}