#include "RigidBody.hpp"
#include <Core/AScene.hh>
#include <Physics/BulletDynamicManager.hpp>
#include <Physics/DynamicMotionState.hpp>
#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	RigidBody::RigidBody()
		: ComponentBase(),
		_collisionShape(nullptr),
		_motionState(nullptr),
		_rigidBody(nullptr),
		_manager(nullptr),
		_mass(0.0f),
		_inertia(btVector3(0.0f, 0.0f, 0.0f)),
		_rotationConstraint(glm::uvec3(1, 1, 1)),
		_transformConstraint(glm::uvec3(1, 1, 1)),
		_shapeType(UndefinedTypeId),
		_shapePath(""),
		_collisionShapeType(UNDEFINED)
	{
	}

	void RigidBody::init(float mass/* = 1.0f*/)
	{
		auto scene = entity.getScene();
		_manager = dynamic_cast<BulletDynamicManager*>(scene->getInstance<BulletCollisionManager>());
		assert(_manager != nullptr);
		_mass = mass;
	}

	void RigidBody::reset()
	{
		_clearBulletObjects();

		_shapeType = UNDEFINED;
		_mass = 0.0f;
		_rotationConstraint = glm::uvec3(1, 1, 1);
		_transformConstraint = glm::uvec3(1, 1, 1);
		_inertia.setValue(0, 0, 0);
	}

	void RigidBody::setTransformation(const AGE::Link *link)
	{
		if (_shapeType == UNDEFINED || !_rigidBody)
			return;
		btTransform tt = _rigidBody->getCenterOfMassTransform();
		tt.setOrigin(convertGLMVectorToBullet(link->getPosition()));
		glm::quat rot = link->getOrientation();
		tt.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		_rigidBody->setWorldTransform(tt);
		_collisionShape->setLocalScaling(convertGLMVectorToBullet(link->getScale()));
	}

	btMotionState &RigidBody::getMotionState()
	{
		assert(_motionState != nullptr && "Motion state is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
		return *_motionState;
	}

	btCollisionShape &RigidBody::getShape()
	{
		assert(_collisionShape != nullptr && "Shape is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
		return *_collisionShape;
	}

	btRigidBody &RigidBody::getBody()
	{
		assert(_rigidBody != nullptr && "RigidBody is NULL. Tips : Have you setAcollisionShape to Component ?.");
		return *_rigidBody;
	}

	void RigidBody::setMass(float mass)
	{
		if (_rigidBody)
		{
			_rigidBody->setMassProps(mass, _inertia);
			_rigidBody->activate(true);
		}
		_mass = btScalar(mass);
	}

	float RigidBody::getMass() const
	{
		return _mass;
	}

	void RigidBody::setInertia(const glm::vec3 &inertia)
	{
		_inertia = convertGLMVectorToBullet(inertia);
		if (_rigidBody)
		{
			_rigidBody->setMassProps(_mass, _inertia);
		}
	}

	void RigidBody::setCollisionMesh(
		const std::string &meshPath
		, short filterGroup /*= 1*/
		, short filterMask /*= -1*/)
	{
		_clearBulletObjects();

		_shapePath = meshPath;

		auto e = entity;
		_motionState = _manager->getObjectPool().create<DynamicMotionState>(&this->entity.getLink());
		auto media = _manager->loadShape(meshPath);
		if (!media)
			return;
		auto s = dynamic_cast<btConvexHullShape*>(media.get());
		if (s) // dynamic
		{
			_collisionShape = _manager->getObjectPool().create<btConvexHullShape>(*s);
			_shapeType = TypeID::Get<btConvexHullShape>();
		}
		else // static
		{
			auto m = dynamic_cast<btBvhTriangleMeshShape*>(media.get());
			_collisionShape = _manager->getObjectPool().create<btScaledBvhTriangleMeshShape>(m, btVector3(1, 1, 1));
			_shapeType = TypeID::Get<btScaledBvhTriangleMeshShape>();
		}
		
		if (_mass != 0)
			_collisionShape->calculateLocalInertia(_mass, _inertia);
		if (((size_t)(this) & 0x3) == 0)
		{
			int i = 0;
		}
		else
		{
		//	assert(false);
		}
		_rigidBody = _manager->getObjectPool().create<btRigidBody>(_mass, _motionState, _collisionShape, _inertia);
		_rigidBody->setUserPointer((void*)(entity.getPtr()));
		_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
		_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));

		if (_rigidBody->isStaticObject())
		{
			//_rigidBody->setActivationState(DISABLE_SIMULATION);
		}
		_manager->getWorld()->addRigidBody(_rigidBody, filterGroup, filterMask);
		setTransformation(&e.getLink());
	}

	void RigidBody::setCollisionShape(
		CollisionShape c
		, short filterGroup /*= 1*/
		, short filterMask /* = -1*/)
	{
		if (c == UNDEFINED)
			return;
		_shapePath = "";
		_clearBulletObjects();

		auto e = entity;
		_motionState = _manager->getObjectPool().create<DynamicMotionState>(&e.getLink());
		if (c == BOX)
		{
			_collisionShape = _manager->getObjectPool().create<btBoxShape>(btVector3(0.5, 0.5, 0.5));
			_shapeType = TypeID::Get<btBoxShape>();
		}
		else if (c == SPHERE)
		{
			_collisionShape = _manager->getObjectPool().create<btSphereShape>(btSphereShape(0.5));
			_shapeType = TypeID::Get<btSphereShape>();
		}
		else
		{
			assert(false);
		}

		if (_mass != 0)
			_collisionShape->calculateLocalInertia(_mass, _inertia);
		_rigidBody = _manager->getObjectPool().create<btRigidBody>(_mass, _motionState, _collisionShape, _inertia);
		_rigidBody->setUserPointer((void*)(entity.getPtr()));
		_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
		_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));

		if (_rigidBody->isStaticObject())
		{
			//_rigidBody->setActivationState(DISABLE_SIMULATION);
		}
		_manager->getWorld()->addRigidBody(_rigidBody, filterGroup, filterMask);
		setTransformation(&e.getLink());
	}

	void RigidBody::setRotationConstraint(bool x, bool y, bool z)
	{
		_rotationConstraint = glm::uvec3(static_cast<unsigned int>(x),
			static_cast<unsigned int>(y),
			static_cast<unsigned int>(z));
		if (!_rigidBody)
			return;
		_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
	}

	void RigidBody::_clearBulletObjects()
	{
		if (_rigidBody != nullptr)
		{
			_manager->getWorld()->removeRigidBody(_rigidBody);
			_manager->getObjectPool().destroy<btRigidBody>(_rigidBody);
		}
		_rigidBody = nullptr;

		if (_motionState != nullptr)
		{
			_manager->getObjectPool().destroy<DynamicMotionState>((DynamicMotionState*)_motionState);
		}
		_motionState = nullptr;

		if (_collisionShape != nullptr)
		{
			if (_shapeType == TypeID::Get<btScaledBvhTriangleMeshShape>())
				_manager->getObjectPool().destroy<btScaledBvhTriangleMeshShape>((btScaledBvhTriangleMeshShape*)_collisionShape);
			else if (_shapeType == TypeID::Get<btConvexHullShape>())
				_manager->getObjectPool().destroy<btConvexHullShape>((btConvexHullShape*)_collisionShape);
			else if (_shapeType == TypeID::Get<btSphereShape>())
				_manager->getObjectPool().destroy<btSphereShape>((btSphereShape*)_collisionShape);
			else if (_shapeType == TypeID::Get<btBoxShape>())
				_manager->getObjectPool().destroy<btBoxShape>((btBoxShape*)_collisionShape);
			else
				assert(false);
		}
		_collisionShape = nullptr;
	}

	void RigidBody::_addContactInformation(const Entity &entity, const btVector3 &contactPoint, const btVector3 &contactNormal)
	{
		ContactInformationsType::iterator found = _contactInformations.find(entity);
		if (found != _contactInformations.end())
		{
			found->second.emplace_back(contactPoint, contactNormal);
		}
		else
		{
			_contactInformations.emplace(entity, ContactInformationList()).first->second.emplace_back(contactPoint, contactNormal);
		}
	}

	const RigidBody::ContactInformationsType &RigidBody::getContactInformations(void) const
	{
		return _contactInformations;
	}

	void RigidBody::setTransformConstraint(bool x, bool y, bool z)
	{
		_transformConstraint = glm::uvec3(static_cast<unsigned int>(x),
			static_cast<unsigned int>(y),
			static_cast<unsigned int>(z));
		if (!_rigidBody)
			return;
		_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));
	}

	RigidBody::~RigidBody(void)
	{
	}

	void RigidBody::postUnserialization()
	{
		_manager = dynamic_cast<BulletDynamicManager*>(entity.getScene()->getInstance<BulletCollisionManager>());
#ifdef EDITOR_ENABLED
		if (WESerialization::SerializeForEditor())
		{
			if (_collisionShapeType != UNDEFINED)
			{
				setCollisionShape((CollisionShape)_collisionShapeType);
			}
			else if (!selectedShapePath.empty())
			{
				setCollisionMesh(selectedShapePath);
			}
		}
		else		
		{
			if (_collisionShapeType != UNDEFINED)
			{
				setCollisionShape((CollisionShape)_collisionShapeType);
			}
			else if (!_shapePath.empty())
			{
				setCollisionMesh(_shapePath);
			}
		}
#else
		if (_collisionShapeType != UNDEFINED)
		{
			setCollisionShape((CollisionShape)_collisionShapeType);
		}
		else if (!_shapePath.empty())
		{
			setCollisionMesh(_shapePath);
		}
#endif

	}

#ifdef EDITOR_ENABLED
	void RigidBody::editorCreate()
	{
		setMass(0.0f);
	}

	void RigidBody::editorDelete()
	{}

	bool RigidBody::editorUpdate()
	{
		bool modified = false;

		if (ImGui::InputFloat("Mass", &_mass))
		{
			setMass(_mass);
			modified = true;
		}

		ImGui::Text("Rotation constraint");
		ImGui::SameLine();
		bool rotationConstraint = false;
		rotationConstraint ^= ImGui::CheckboxFlags("x", &(_rotationConstraint.x), 0);
		ImGui::SameLine();
		rotationConstraint ^= ImGui::CheckboxFlags("y", &(_rotationConstraint.y), 0);
		ImGui::SameLine();
		rotationConstraint ^= ImGui::CheckboxFlags("z", &(_rotationConstraint.z), 0);

		if (rotationConstraint)
		{
			setRotationConstraint(_rotationConstraint.x == 1, _rotationConstraint.y == 1, _rotationConstraint.z == 1);
			modified = true;
		}

		ImGui::Text("Transformation constraint");
		ImGui::SameLine();
		bool transformConstraint = false;
		transformConstraint ^= ImGui::CheckboxFlags("x", &(_transformConstraint.x), 0);
		ImGui::SameLine();
		transformConstraint ^= ImGui::CheckboxFlags("y", &(_transformConstraint.y), 0);
		ImGui::SameLine();
		transformConstraint ^= ImGui::CheckboxFlags("z", &(_transformConstraint.z), 0);

		if (transformConstraint)
		{
			setTransformConstraint(_transformConstraint.x == 1, _transformConstraint.y == 1, _transformConstraint.z == 1);
			modified = true;
		}

		//ImGui::InputFloat3("Rotation constraint", glm::value_ptr(_rotationConstraint));
		//ImGui::InputFloat3("Transform constraint", glm::value_ptr(_transformConstraint));


		if ((*shapePathList)[selectedShapeIndex] != selectedShapePath)
		{
			std::size_t i = 0;
			for (auto &e : *shapePathList)
			{
				if (e == selectedShapePath)
				{
					selectedShapeIndex = i;
					break;
				}
				++i;
			}
		}

		if(ImGui::Checkbox("Simple shape", &simpleShapes))
		{
			modified = true;
			if (simpleShapes)
			{
				reset();
				selectedShapeName = "";
				selectedShapePath = "";
				selectedShapeIndex = 0;
			}
			else
			{
				reset();
				_collisionShapeType = UNDEFINED;
			}
		}

		if (!simpleShapes) // if mesh
		{
			ImGui::PushItemWidth(-1);
			if (ImGui::ListBox("Shapes", (int*)&selectedShapeIndex, &(shapeFileList->front()), (int)(shapeFileList->size())))
			{
				modified = true;
				selectedShapeName = (*shapeFileList)[selectedShapeIndex];
				selectedShapePath = (*shapePathList)[selectedShapeIndex];

				setCollisionMesh(selectedShapePath);
			}
			ImGui::PopItemWidth();
		}
		else
		{
			static const char *CollisionShapeStr[] = { "Sphere", "Box" };
			ImGui::PushItemWidth(-1);
			if (ImGui::ListBox("Shapes", (int*)&selectedShapeIndex, CollisionShapeStr, CollisionShape::UNDEFINED))
			{
				modified = true;
				_collisionShapeType = (CollisionShape)selectedShapeIndex;
				setCollisionShape((CollisionShape)selectedShapeIndex);
			}
			ImGui::PopItemWidth();
		}
		return modified;
	}
#endif
}
