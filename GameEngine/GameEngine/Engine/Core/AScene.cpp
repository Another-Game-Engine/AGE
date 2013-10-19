
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
	std::list<SmartPointer<Components::AComponent> >::iterator	it;

	it = _components.begin();
	while (it != _components.end())
	{
		(*it)->update();
		++it;
	}
}

void 							AScene::recomputePositions()
{
	recomputePositions(_root, false);
}
