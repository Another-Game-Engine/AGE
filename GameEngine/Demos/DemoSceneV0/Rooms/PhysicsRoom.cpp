#include "PhysicsRoom.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Systems/HotZoneSystem.hpp>
#include <Components/CollisionBody.hpp>

	PhysicsRoom::PhysicsRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	PhysicsRoom::~PhysicsRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool PhysicsRoom::init()
	{
		// load
		auto s = _scene.lock();
		hotZonePhysicsProjection = createHotZone("Physics->Projection", "HZ-projection-physics");
		return true;
	}

	bool PhysicsRoom::_enable()
	{
		auto scene = _scene.lock();
		{
			glm::mat4 t(1);
			t = glm::translate(t, glm::vec3(-14.5, -0.1f, -27.1));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
			auto s = 3;
			for (auto i = 0; i < s; i++)
			{
				for (auto j = 0; j < s; ++j)
				{
					for (auto k = 0; k < s; ++k)
					{
						auto e = scene->createEntity();
						e->setLocalTransform(t);
						e->setLocalTransform(glm::scale(t, glm::vec3(0.2f)));
						e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(((float)(s) - (float)(j)) + 0.05f , s - i, ((float)(s) - (float)(k)) + 0.05f)));
						//
						e->addComponent<Component::MeshRenderer>(meshObj)->setShader("MaterialBasic");
						e->addComponent<Component::RigidBody>(1.0f)->setCollisionShape(Component::RigidBody::BOX);
						cubes.push_back(e);
					}
				}
			}
		}
		return true;
	}

	bool PhysicsRoom::_disable()
	{
		auto scene = _scene.lock();
		for (auto e : cubes)
		{
			scene->destroy(e);
		}
		cubes.clear();
		return true;
	}