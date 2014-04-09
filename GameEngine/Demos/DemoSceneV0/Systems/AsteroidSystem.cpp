#include <Systems/AsteroidSystem.hh>

#include <Components/MeshRenderer.hh>
#include <Components/RigidBody.hpp>
#include <Components/Collision.hpp>
#include <MediaFiles/AssetsManager.hpp>

#include <Physic/Utils/BtConversion.hpp>

#include "MyTags.hpp"

#include <SDL/SDL.h>
#include <glm/glm.hpp>

AsteroidSystem::AsteroidSystem(std::weak_ptr<AScene> &&scene)
: System(std::move(scene)),
_asteroidFilter(std::move(scene)),
_bulletFilter(std::move(scene))
{
}

AsteroidSystem::~AsteroidSystem()
{
}

bool AsteroidSystem::initialize()
{
	_asteroidFilter.requireTag(MyTags::ASTEROID_TAG);
	_asteroidFilter.requireComponent<Component::Collision>();
	_bulletFilter.requireTag(MyTags::BULLET_TAG);
	_bulletFilter.requireComponent<Component::Collision>();
	return true;
}

void AsteroidSystem::mainUpdate(double time)
{
	std::set<Entity, bool(*)(const Entity&, const Entity&)>::iterator eIt = _asteroidFilter.getCollection().begin();

	while (eIt != _asteroidFilter.getCollection().end())
	{
		bool toRm = false;
		Entity e = *eIt;
		std::set<Entity>::iterator cIt = e->getComponent<Component::Collision>()->collisions.begin();

		float mass = 1.0f / e->getComponent<Component::RigidBody>()->getBody().getInvMass();

		while (cIt != e->getComponent<Component::Collision>()->collisions.end())
		{
			Entity c = *cIt;
			if (c.get() != NULL && c->isTagged(MyTags::BULLET_TAG))
			{
				toRm = true;
				glm::vec3 impulse = glm::vec3(rand() % 10000, rand() % 10000, rand() % 10000);

				impulse = glm::normalize(impulse);
				impulse *= 1000.0f;

				Entity asteroid1, asteroid2;

				asteroid1 = _scene.lock()->createEntity();
				asteroid2 = _scene.lock()->createEntity();

				asteroid1->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.5f)));
				asteroid2->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.5f)));

				auto mesh = asteroid1->addComponent<Component::MeshRenderer>(_scene.lock()->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
				mesh->setShader("MaterialBasic");
				auto body = asteroid1->addComponent<Component::RigidBody>(mass / 2.0f);
				body->setCollisionShape(Component::RigidBody::SPHERE);
				body->getBody().setRestitution(1.0f);
				body->getBody().setActivationState(DISABLE_DEACTIVATION);
				body->getBody().setDamping(0.0f, 0.0f);
				body->getBody().applyCentralImpulse(convertGLMVectorToBullet(impulse));
				asteroid1->addTag(MyTags::ASTEROID_TAG);

				mesh = asteroid2->addComponent<Component::MeshRenderer>(_scene.lock()->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
				mesh->setShader("MaterialBasic");
				body = asteroid2->addComponent<Component::RigidBody>(mass / 2.0f);
				body->setCollisionShape(Component::RigidBody::SPHERE);
				body->getBody().setRestitution(1.0f);
				body->getBody().setActivationState(DISABLE_DEACTIVATION);
				body->getBody().setDamping(0.0f, 0.0f);
				body->getBody().applyCentralImpulse(convertGLMVectorToBullet(impulse));
				asteroid2->addTag(MyTags::ASTEROID_TAG);

				cIt = e->getComponent<Component::Collision>()->collisions.erase(cIt);
				_scene.lock()->destroy(c);
			}
			else
				++cIt;
		}
		++eIt;
		if (toRm)
			_scene.lock()->destroy(e);
	}
}
