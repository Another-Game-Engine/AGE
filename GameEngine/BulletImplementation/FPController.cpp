#include <Components/FPController.hpp>
#include "Context/SdlContext.hh"

using namespace Component;

FPController::FPController(Engine &engine, Handle &entity) : ComponentBase<FPController>(engine, entity, "KinematicCharacterController")
, _controller(nullptr)
, _ghost(nullptr)
, _shape(nullptr)
, _orientation(glm::vec3(0,0,0))
, _forwardWalkSpeed(4.0f)
, _forwardRunSpeed(7.0f)
, _backwardWalkSpeed(4.0f)
, _backwardRunSpeed(7.0f)
, _sideWalkSpeed(3.0f)
, _sideRunSpeed(5.0f)
, _rotateXSpeed(1.0f)
, _rotateYSpeed(1.0f)
, _jumpSpeed(2.0f)
, _jumpHeight(3.0f)
, _canJump(true)
, _canRun(true)
, _leftKey(SDLK_a)
, _rightKey(SDLK_d)
, _forwarKey(SDLK_w)
, _backWardKey(SDLK_s)
, _jumpKey(SDLK_SPACE)
, _runKey(SDLK_LSHIFT)
{
	btTransform transform;
	glm::vec3 position = posFromMat4(_entity->getLocalTransform());
	glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
	std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
	glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
	transform.setIdentity();
	transform.setOrigin(convertGLMVectorToBullet(position));
	transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

	_ghost = new btPairCachingGhostObject();
	_shape = new btCapsuleShape(1, 1);
	_ghost->setCollisionShape(_shape);
	_ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	_ghost->setWorldTransform(transform);
	_controller = new btKinematicCharacterController(_ghost, _shape, btScalar(0.35));
	//_controller->setGravity(- _engine.getInstance<BulletManager>().getWorld().getGravity().y());
	//				_engine.getInstance<BulletManager>().getWorld().addCharacter(_controller);
	_ghost->setUserPointer(&(_entity));
	_engine.getInstance<BulletManager>().getWorld().addCollisionObject(_ghost, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	_engine.getInstance<BulletManager>().getWorld().addAction(_controller);
	_engine.getInstance<BulletManager>().getWorld().getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}

FPController::~FPController()
{
}

void FPController::init()
{}

void FPController::reset()
{
}

void FPController::clear()
{
}

btGhostObject &FPController::getGhost()
{
	return *_ghost;
}

btKinematicCharacterController &FPController::getController()
{
	return *_controller;
}
