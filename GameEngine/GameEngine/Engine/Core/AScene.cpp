
#include "AScene.hh"

AScene::AScene() :
	_root(new Entity),
	_camera(NULL)
{
}

AScene::~AScene()
{
	if (_camera)
		delete _camera;
	_root = NULL;
}

void 							AScene::recomputePositions(SmartPointer<Entity> &father,
															bool hasMoved)
{
	Entity::t_sonsList::iterator 	it = father->getSonsBegin();

	while (it != father->getSonsEnd())
	{

		/////
		/////   UPDATE SYSTEMS HERE !!!! TODO
		/////

		//Entity::t_ComponentsList::iterator		comp = it->second->getComponentsBegin();

		//while (comp != it->second->getComponentsEnd())
		//{
		//	comp->second->update(); // update components
		//	++comp;
		//}
		if (it->second->getFlags() & Entity::HAS_MOVED)
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
	_root->computeGlobalTransform(glm::mat4(1));
	recomputePositions(_root, false);
}

SmartPointer<Entity>            &AScene::createEntity()
{
	SmartPointer<Entity> tmp = new Entity();

	_root->addSon(tmp);
	return tmp;
}