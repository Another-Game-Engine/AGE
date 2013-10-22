
#include "AScene.hh"

AScene::AScene() :
	_root(new Entity),
	_camera(NULL)
{
}

AScene::~AScene()
{
}

void 							AScene::recomputePositions(SmartPointer<Entity> &father,
															bool hasMoved)
{
	Entity::t_sonsList::iterator 	it = father->getSonsBegin();

	while (it != father->getSonsEnd())
	{
		Entity::t_ComponentsList::iterator		comp = it->second->getComponentsBegin();

		while (comp != it->second->getComponentsEnd())
		{
			comp->second->update(); // update components
			++comp;
		}
		if (father->getFlags() & Entity::HAS_MOVED)
			hasMoved = true;
		if (hasMoved)
			it->second->computeGlobalTransform(father->getGlobalTransform());
		recomputePositions(it->second, hasMoved);
		++it;
	}
}

SmartPointer<Entity>	const	&AScene::getRoot()
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