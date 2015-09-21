#include <ComponentsCore/RigidBody.hpp>
#include <ComponentsCore/Collider.hpp>
#include <ComponentsCore/PhysicsData.hpp>
#include <Core/AScene.hh>
#include <Physics/PhysicsInterface.hpp>
#include <Physics/WorldInterface.hpp>
#include <Utils/MatrixConversion.hpp>
#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	// Methods
	void RigidBody::init(void)
	{
		assert(rigidBody == nullptr && "RigidBody already initialized");
		rigidBody = entity->getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->createRigidBody(entity->addComponent<Private::PhysicsData>(entity)->getData());
		rigidBody->rigidBody = this;
		Link *link = entity.getLinkPtr();
		setPosition(link->getPosition());
		setRotation(link->getOrientation());
	}

	void RigidBody::postUnserialization()
	{
		Link *link = entity.getLinkPtr();
		if (isKinematic())
		{
			auto p = posFromMat4(link->getGlobalTransform());
			setPosition(posFromMat4(link->getGlobalTransform()));
			setRotation(link->getOrientation());
		}
	}

	void RigidBody::setAngularDrag(float angularDrag)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setAngularDrag(angularDrag);
	}

	float RigidBody::getAngularDrag(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getAngularDrag();
	}

	void RigidBody::setAngularVelocity(const glm::vec3 &angularVelocity)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setAngularVelocity(angularVelocity);
	}

	glm::vec3 RigidBody::getAngularVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getAngularVelocity();
	}

	void RigidBody::setCenterOfMass(const glm::vec3 &centerOfMass)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setCenterOfMass(centerOfMass);
	}

	glm::vec3 RigidBody::getCenterOfMass(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getCenterOfMass();
	}

	void RigidBody::setLinearDrag(float linearDrag)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setLinearDrag(linearDrag);
	}

	float RigidBody::getLinearDrag(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getLinearDrag();
	}

	void RigidBody::setLinearVelocity(const glm::vec3 &linearVelocity)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setLinearVelocity(linearVelocity);
	}

	glm::vec3 RigidBody::getLinearVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getLinearVelocity();
	}

	void RigidBody::setMass(float mass)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setMass(mass);
	}

	float RigidBody::getMass(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getMass();
	}

	void RigidBody::setDiagonalInertiaTensor(const glm::vec3 &diagonalInertiaTensor)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setDiagonalInertiaTensor(diagonalInertiaTensor);
	}

	glm::vec3 RigidBody::getDiagonalInertiaTensor(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getDiagonalInertiaTensor();
	}

	void RigidBody::setMaxAngularVelocity(float maxAngularVelocity)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setMaxAngularVelocity(maxAngularVelocity);
	}

	float RigidBody::getMaxAngularVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getMaxAngularVelocity();
	}

	void RigidBody::setMaxDepenetrationVelocity(float maxDepenetrationVelocity)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
	}

	float RigidBody::getMaxDepenetrationVelocity(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getMaxDepenetrationVelocity();
	}

	void RigidBody::setPosition(const glm::vec3 &position)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setPosition(position);
	}

	glm::vec3 RigidBody::getPosition(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getPosition();
	}

	void RigidBody::setRotation(const glm::quat &rotation)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setRotation(rotation);
	}

	glm::quat RigidBody::getRotation(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getRotation();
	}

	void RigidBody::affectByGravity(bool mustBeAffectedByGravity)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->affectByGravity(mustBeAffectedByGravity);
	}

	bool RigidBody::isAffectedByGravity(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->isAffectedByGravity();
	}

	void RigidBody::setAsKinematic(bool mustBeKinematic)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setAsKinematic(mustBeKinematic);
	}

	bool RigidBody::isKinematic(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->isKinematic();
	}

	void RigidBody::setCollisionDetectionMode(Physics::CollisionDetectionMode collisionDetectionMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->setCollisionDetectionMode(collisionDetectionMode);
	}

	Physics::CollisionDetectionMode RigidBody::getCollisionDetectionMode(void) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getCollisionDetectionMode();
	}

	void RigidBody::addExplosionForce(float explosionForce, const glm::vec3 &explosionPosition, float explosionRadius, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->addExplosionForce(explosionForce, explosionPosition, explosionRadius, forceMode);
	}

	void RigidBody::addForce(const glm::vec3 &force, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->addForce(force, forceMode);
	}

	void RigidBody::addForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &position, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->addForceAtWorldPosition(force, position, forceMode);
	}

	void RigidBody::addForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &position, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->addForceAtLocalPosition(force, position, forceMode);
	}

	void RigidBody::addAbsoluteTorque(const glm::vec3 &torque, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->addAbsoluteTorque(torque, forceMode);
	}

	void RigidBody::addRelativeTorque(const glm::vec3 &torque, Physics::ForceMode forceMode)
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		rigidBody->addRelativeTorque(torque, forceMode);
	}

	glm::vec3 RigidBody::getVelocityAtWorldPosition(const glm::vec3 &position) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getVelocityAtWorldPosition(position);
	}

	glm::vec3 RigidBody::getVelocityAtLocalPosition(const glm::vec3 &position) const
	{
		assert(rigidBody != nullptr && "Invalid RigidBody");
		return rigidBody->getVelocityAtLocalPosition(position);
	}

	// Inherited Methods
	void RigidBody::reset(void)
	{
		if (rigidBody != nullptr)
		{
			entity->getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->destroyRigidBody(rigidBody);
			rigidBody = nullptr;
		}
		if (!entity->haveComponent<Collider>())
		{
			entity->removeComponent<Private::PhysicsData>();
		}
	}

#ifdef EDITOR_ENABLED

	void RigidBody::editorCreate(void)
	{
		return;
	}

	void RigidBody::editorDelete(void)
	{
		return;
	}

	bool RigidBody::editorUpdate(void)
	{
		if (editorStruct == nullptr)
		{
			editorStruct = std::make_unique<EditorStruct>();
		}
		editorStruct->copyDatas(this);
		editorStruct->editorUpdate(this);
		return false;
	}

	void RigidBody::EditorStruct::copyDatas(RigidBody *ptr)
	{
		angularDrag = ptr->getAngularDrag();
		angularVelocity = ptr->getAngularVelocity();
		centerOfMass = ptr->getCenterOfMass();
		linearDrag = ptr->getLinearDrag();
		linearVelocity = ptr->getLinearVelocity();
		mass = ptr->getMass();
		diagonalInertiaTensor = ptr->getDiagonalInertiaTensor();
		maxAngularVelocity = ptr->getMaxAngularVelocity();
		maxDepenetrationVelocity = ptr->getMaxDepenetrationVelocity();
		isAffectedByGravity = ptr->isAffectedByGravity();
		kinematic = ptr->isKinematic();
		collisionDetectionMode = ptr->getCollisionDetectionMode();
	}

	void RigidBody::EditorStruct::refreshDatas(RigidBody *ptr)
	{
		ptr->setAngularDrag(angularDrag);
		ptr->setAngularVelocity(angularVelocity);
		ptr->setCenterOfMass(centerOfMass);
		ptr->setLinearDrag(linearDrag);
		ptr->setLinearVelocity(linearVelocity);
		ptr->setMass(mass);
		ptr->setDiagonalInertiaTensor(diagonalInertiaTensor);
		ptr->setMaxAngularVelocity(maxAngularVelocity);
		ptr->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
		ptr->affectByGravity(isAffectedByGravity);
		ptr->setAsKinematic(kinematic);
		ptr->setCollisionDetectionMode(collisionDetectionMode);
	}


	void RigidBody::EditorStruct::editorUpdate(RigidBody *ptr)
	{
		if (ImGui::InputFloat("Angular Drag", &angularDrag))
		{
			ptr->setAngularDrag(angularDrag);
		}
		if (ImGui::InputFloat3("Angular Velocity", glm::value_ptr(angularVelocity)))
		{
			ptr->setAngularVelocity(angularVelocity);
		}
		if (ImGui::InputFloat3("Center of Mass", glm::value_ptr(centerOfMass)))
		{
			ptr->setCenterOfMass(centerOfMass);
		}
		if (ImGui::InputFloat("Linear Drag", &linearDrag))
		{
			ptr->setLinearDrag(linearDrag);
		}
		if (ImGui::InputFloat3("Linear Velocity", glm::value_ptr(linearVelocity)))
		{
			ptr->setLinearVelocity(linearVelocity);
		}
		if (ImGui::InputFloat("Mass", &mass))
		{
			ptr->setMass(mass);
		}
		if (ImGui::InputFloat3("Diagonal Inertia Tensor", glm::value_ptr(diagonalInertiaTensor)))
		{
			ptr->setDiagonalInertiaTensor(diagonalInertiaTensor);
		}
		if (ImGui::InputFloat("Max Angular Velocity", &maxAngularVelocity))
		{
			ptr->setMaxAngularVelocity(maxAngularVelocity);
		}
		if (ImGui::InputFloat("Max Depenetration Velocity", &maxDepenetrationVelocity))
		{
			ptr->setMaxDepenetrationVelocity(maxDepenetrationVelocity);
		}
		if (ImGui::Checkbox("Is Affected By Gravity", &isAffectedByGravity))
		{
			ptr->affectByGravity(isAffectedByGravity);
		}
		if (ImGui::Checkbox("Is Kinematic", &kinematic))
		{
			ptr->setAsKinematic(kinematic);
		}
		static const std::string collisionDetectionModeNames[] =
		{
			"Discrete",
			"Continuous"
		};
		ImGui::Text("Collision Detection Mode");
		for (unsigned int index = 0; index < sizeof(collisionDetectionModeNames) / sizeof(*collisionDetectionModeNames); ++index)
		{
			if (ImGui::RadioButton(collisionDetectionModeNames[index].c_str(), static_cast<unsigned int>(collisionDetectionMode) == index))
			{
				collisionDetectionMode = static_cast<Physics::CollisionDetectionMode>(index);
				ptr->setCollisionDetectionMode(collisionDetectionMode);
			}
		}
	}

#endif

	void RigidBody::_copyFrom(const ComponentBase *model)
	{
		init();
		auto m = (RigidBody*)model;
#ifdef EDITOR_ENABLED
		editorUpdate();
		if (editorStruct)
		{
			editorStruct->copyDatas(m);
			editorStruct->refreshDatas(this);
			Link *link = entity.getLinkPtr();
			if (isKinematic())
			{
				auto p = posFromMat4(link->getGlobalTransform());
				setPosition(posFromMat4(link->getGlobalTransform()));
				setRotation(link->getOrientation());
				// global orientation not supported
			}
		}
#endif
	}
}