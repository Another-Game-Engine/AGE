#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>
#include <Components/RigidBody.hpp>
#include <MyTags.hpp>

class PistolSystem : public System
{
public:
	PistolSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _bullets(scene)
		, _characterController(scene)
	{}

	virtual ~PistolSystem()
	{}

private:
	EntityFilter _bullets;
	EntityFilter _characterController;
	std::shared_ptr<Input> _inputs;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float ftime = (float)(time);
		static float delay = 0.0f;
		auto scene = _scene.lock();
		if (_inputs->getKey(SDLK_r) && delay <= 0.0f)
		{
			for (auto controller : _characterController.getCollection())
			{
				glm::vec3 from, to;
				scene->getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
				auto e = scene->createEntity();
				e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(from + to * 1.5f)));
				e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.23f)));
				auto mesh = e->addComponent<Component::MeshRenderer>(scene->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
				mesh->setShader("MaterialBasic");
				auto rigidBody = e->addComponent<Component::RigidBody>(0.4f);
				rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
				rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(to * 8.0f));
				rigidBody->getBody().setFriction(1.0f);
				rigidBody->getBody().setRestitution(0.9f);
			}
			delay = 0.1f;
		}
		if (delay >= 0.0f)
			delay -= ftime;
	}

	virtual void initialize()
	{
		_bullets.requireTag(MyTags::BULLET_TAG);
		_characterController.requireTag(MyTags::HEROS_TAG);
		_inputs = _scene.lock()->getInstance<Input>();
	}
};