#include <Components/FPController.hpp>
#include <Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Components/Extras/CustomCharacterController.hh>

using namespace Component;

FPController::FPController() : ComponentBase<FPController>()
, _controller(nullptr)
, _ghost(nullptr)
, _shape(nullptr)
, yOrientation(0.0f)
, forwardWalkSpeed(8.0f)
, forwardRunSpeed(15.0f)
, backwardWalkSpeed(8.0f)
, backwardRunSpeed(15.0f)
, sideWalkSpeed(5.0f)
, sideRunSpeed(8.0f)
, rotateXSpeed(0.01f)
, rotateYSpeed(0.15f)
, jumpSpeed(2.0f)
, jumpHeight(3.0f)
, canJump(true)
, canRun(true)
, justJump(false)
, justArriveOnFloor(false)
, wasOnGround(false)
{

}

FPController::~FPController()
{
}

void FPController::init()
{
	setKey(LEFT, SDLK_a);
	setKey(RIGHT, SDLK_d);
	setKey(FORWARD, SDLK_w);
	setKey(BACKWARD, SDLK_s);
	setKey(JUMP, SDLK_SPACE);
	setKey(RUN, SDLK_LSHIFT);
	controls.fill(false);

	btTransform transform;
	glm::vec3 position = posFromMat4(_entity->getLocalTransform());
	glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
	glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
	transform.setIdentity();
	transform.setOrigin(convertGLMVectorToBullet(position));
	transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

	_ghost = new btPairCachingGhostObject();
	_shape = new btCylinderShape(convertGLMVectorToBullet(scale));

	_ghost->setCollisionShape(_shape);
	_ghost->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	_ghost->setWorldTransform(transform);
	_ghost->setRestitution(0);
	_ghost->setActivationState(DISABLE_DEACTIVATION);
	_ghost->setUserPointer(&(_entity));
	_controller = new btKinematicCharacterController(_ghost, _shape, 1);
	auto bulletManager = dynamic_cast<BulletDynamicManager*>(&(_entity->getScene()->getEngine().getInstance<BulletCollisionManager>()));
	assert(bulletManager != nullptr);
	bulletManager->getWorld()->addCollisionObject(_ghost, btBroadphaseProxy::KinematicFilter);
	bulletManager->getWorld()->addAction(_controller);
	bulletManager->getWorld()->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	justJump = false;
	justArriveOnFloor = false;
	wasOnGround = true;
}

void FPController::reset()
{	auto bulletManager = dynamic_cast<BulletDynamicManager*>(&(_entity->getScene()->getEngine().getInstance<BulletCollisionManager>()));
	assert(bulletManager != nullptr);

	if (_controller)
	{
		bulletManager->getWorld()->removeAction(_controller);
		delete _controller;
	}
	if (_shape)
	{
		delete _shape;
	}
	if (_ghost)
	{
		bulletManager->getWorld()->removeCollisionObject(_ghost);
		delete _ghost;
	}
}

void FPController::clear()
{
}

btKinematicCharacterController &FPController::getController()
{
	return *_controller;
}

btGhostObject &FPController::getGhost()
{
	return *_ghost;
}

btConvexShape &FPController::getShape()
{
	return *_shape;
}

void FPController::setKey(FPController::CONTROLS k, unsigned int key)
{
	if (k >= 6)
		return;
	keys[k] = key;
}

void FPController::resetControls()
{
	controls.fill(false);
}