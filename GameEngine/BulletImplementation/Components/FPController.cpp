#include <Components/FPController.hpp>
#include <Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>

using namespace Component;

FPController::FPController(Engine &engine, Handle &entity) : ComponentBase<FPController>(engine, entity, "KinematicCharacterController")
, _controller(nullptr)
, _ghost(nullptr)
, _shape(nullptr)
, _orientation(glm::vec3(0, 0, 0))
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
{
	setKey(LEFT, SDLK_a);
	setKey(RIGHT, SDLK_d);
	setKey(FORWARD, SDLK_w);
	setKey(BACKWARD, SDLK_s);
	setKey(JUMP, SDLK_SPACE);
	setKey(RUN, SDLK_LSHIFT);
	_controls.fill(false);

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

const glm::vec3 &FPController::getOrientation() const
{
	return _orientation;
}

void FPController::setOrientation(const glm::vec3 &v)
{
	_orientation = v;
}

const float FPController::getForwardWalkSpeed() const
{
	return _forwardWalkSpeed;
}

void FPController::setForwardWalkSpeed(float s)
{
	_forwardWalkSpeed = s;
}

const float FPController::getForwardRunSpeed() const
{
	return _forwardRunSpeed;
}

void FPController::setForwardRunSpeed(float s)
{
	_forwardRunSpeed = s;
}

const float FPController::getBackwardWalkSpeed() const
{
	return _backwardWalkSpeed;
}

void FPController::setBackwardWalkSpeed(float s)
{
	_backwardWalkSpeed = s;
}

const float FPController::getBackwardRunSpeed() const
{
	return _backwardRunSpeed;
}

void FPController::setBackwardRunSpeed(float s)
{
	_backwardRunSpeed = s;
}

const float FPController::getSideWalkSpeed() const
{
	return _sideWalkSpeed;
}

void FPController::setSideWalkSpeed(float s)
{
	_sideWalkSpeed = s;
}

const float FPController::getSideRunSpeed() const
{
	return _sideRunSpeed;
}

void FPController::setSideRunSpeed(float s)
{
	_sideRunSpeed = s;
}

const float FPController::getRotateXSpeed() const
{
	return _rotateXSpeed;
}

void FPController::setRotateXSpeed(float s)
{
	_rotateXSpeed = s;
}

const float FPController::getRotateYSpeed() const
{
	return _rotateYSpeed;
}

void FPController::setRotateYSpeed(float s)
{
	_rotateYSpeed = s;
}

const float FPController::getJumpSpeed() const
{
	return _jumpSpeed;
}

void FPController::setJumpSpeed(float s)
{
	_jumpSpeed = s;
}

const float FPController::getJumpHeight() const
{
	return _jumpHeight;
}

void FPController::setJumpHeight(float s)
{
	_jumpHeight = s;
}

bool FPController::canJump() const
{
	return _canJump;
}

void FPController::setCanJump(bool b)
{
	_canJump = b;
}

bool FPController::canRun() const
{
	return _canRun;
}

void FPController::setCanRun(bool b)
{
	_canRun = b;
}

void FPController::setKey(FPController::CONTROLS k, unsigned int key)
{
	if (k >= 6)
		return;
	_keys[k] = key;
}