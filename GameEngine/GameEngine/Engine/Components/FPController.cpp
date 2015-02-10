#include <Components/FPController.hpp>
#include <Physic/Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Components/Extras/CustomCharacterController.hh>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <Physic/BulletDynamicManager.hpp>
#include <SDL/SDL.h>

namespace AGE
{

	FPController::FPController() : ComponentBase()
		, _controller(nullptr)
		, _ghost(nullptr)
		, _shape(nullptr)
		, _manager(nullptr)
		, yOrientation(0.0f)
		, forwardWalkSpeed(0.1f)
		, forwardRunSpeed(0.3f)
		, backwardWalkSpeed(0.1f)
		, backwardRunSpeed(0.3f)
		, sideWalkSpeed(0.1f)
		, sideRunSpeed(0.3f)
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

	FPController::FPController(const FPController &o)
	{
		_entity = o._entity;
		yOrientation = o.yOrientation;
		forwardWalkSpeed = o.forwardWalkSpeed;
		backwardWalkSpeed = o.backwardWalkSpeed;
		forwardRunSpeed = o.forwardRunSpeed;
		backwardRunSpeed = o.backwardRunSpeed;
		sideWalkSpeed = o.sideWalkSpeed;
		sideRunSpeed = o.sideRunSpeed;
		rotateXSpeed = o.rotateXSpeed;
		rotateYSpeed = o.rotateYSpeed;
		jumpSpeed = o.jumpSpeed;
		jumpHeight = o.jumpHeight;
		canJump = o.canJump;
		canRun = o.canRun;
		justJump = o.justJump;
		justArriveOnFloor = o.justArriveOnFloor;
		wasOnGround = o.wasOnGround;
		keys = o.keys;
		controls = o.controls;

		_controller = o._controller;
		_ghost = o._ghost;
		_shape = o._shape;
		_manager = o._manager;
	}

	FPController& FPController::operator=(const FPController &o)
	{
		_entity = o._entity;
		yOrientation = o.yOrientation;
		forwardWalkSpeed = o.forwardWalkSpeed;
		backwardWalkSpeed = o.backwardWalkSpeed;
		forwardRunSpeed = o.forwardRunSpeed;
		backwardRunSpeed = o.backwardRunSpeed;
		sideWalkSpeed = o.sideWalkSpeed;
		sideRunSpeed = o.sideRunSpeed;
		rotateXSpeed = o.rotateXSpeed;
		rotateYSpeed = o.rotateYSpeed;
		jumpSpeed = o.jumpSpeed;
		jumpHeight = o.jumpHeight;
		canJump = o.canJump;
		canRun = o.canRun;
		justJump = o.justJump;
		justArriveOnFloor = o.justArriveOnFloor;
		wasOnGround = o.wasOnGround;
		keys = o.keys;
		controls = o.controls;

		_controller = o._controller;
		_ghost = o._ghost;
		_shape = o._shape;
		_manager = o._manager;
		return *this;
	}

	FPController::~FPController()
	{
	}

	void FPController::init(AScene *scene, const Entity &entity, short filterGroup, short filterMask)
	{
		_entity = entity;
		_manager = dynamic_cast<BulletDynamicManager*>(scene->getInstance<BulletCollisionManager>());
		setKey(LEFT, SDLK_a);
		setKey(RIGHT, SDLK_d);
		setKey(FORWARD, SDLK_w);
		setKey(BACKWARD, SDLK_s);
		setKey(JUMP, SDLK_SPACE);
		setKey(RUN, SDLK_LSHIFT);
		controls.fill(false);

		btTransform transform;
		//auto &entityTransform = scene.lock()->getTransform(entity);
		//glm::vec3 position = posFromMat4(entityTransform);
		//glm::vec3 scale = scaleFromMat4(entityTransform);
		//glm::vec3 rot = rotFromMat4(entityTransform, true);
		//transform.setIdentity();
		//transform.setOrigin(convertGLMVectorToBullet(position));
		//transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

		auto link = scene->getLink(entity);
		transform.setIdentity();
		transform.setOrigin(convertGLMVectorToBullet(link->getPosition()));
		transform.setRotation(convertGLMQuatToBullet(link->getOrientation()));

		_ghost = new btPairCachingGhostObject();
		_shape = new btCylinderShape(convertGLMVectorToBullet(link->getScale()));

		_ghost->setCollisionShape(_shape);
		// _ghost->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
		_ghost->setWorldTransform(transform);
		_ghost->setRestitution(0);
		_ghost->setActivationState(DISABLE_DEACTIVATION);
		_ghost->setUserPointer(&(_entity));
		_controller = new btKinematicCharacterController(_ghost, _shape, btScalar(0.1));
		_manager->getWorld()->addCollisionObject(_ghost, filterGroup, filterMask);
		_manager->getWorld()->addAction(_controller);
		_manager->getWorld()->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		justJump = false;
		justArriveOnFloor = false;
		wasOnGround = true;
	}

	void FPController::reset(AScene *)
	{
		if (_controller)
		{
			_manager->getWorld()->removeAction(_controller);
			delete _controller;
		}
		if (_shape)
		{
			delete _shape;
		}
		if (_ghost)
		{
			_manager->getWorld()->removeCollisionObject(_ghost);
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
}