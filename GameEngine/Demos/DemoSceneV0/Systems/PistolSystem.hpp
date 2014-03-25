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
		, _characterController(scene)
		, _balls(scene)
	{}

	virtual ~PistolSystem()
	{}

private:
	EntityFilter _characterController;
	std::queue<Entity> _shots;
	EntityFilter _balls;
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
				e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(from + to * 3.0f)));
				e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.23f)));
				auto mesh = e->addComponent<Component::MeshRenderer>(scene->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
				mesh->setShader("MaterialBasic");
				auto rigidBody = e->addComponent<Component::RigidBody>(0.4f);
				rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
				rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(to * 8.0f));
				rigidBody->getBody().setFriction(1.0f);
				rigidBody->getBody().setRestitution(1.0f);
				e->addTag(MyTags::BULLET_TAG);
				_shots.push(e);
			}
			delay = 0.1f;
		}
		if (delay >= 0.0f)
			delay -= ftime;

		while (_shots.size() > 50)
		{
			scene->destroy(_shots.front());
			_shots.pop();
		}
		for (auto e : _balls.getCollection())
		{
			if (e->hasComponent<Component::Collision>() && e->isTagged(MyTags::BULLET_TAG))
			{
				auto b = e->isTagged(MyTags::HEROS_TAG);
				auto c = e->isTagged(MyTags::NO_TAG);
				auto d = e->isTagged(MyTags::SLIDE_TAG);


				auto a = e.get();
				std::cout << a << std::endl;
			}
			if (e->hasComponent<Component::PointLight>())
				e->removeComponent<Component::PointLight>();
			else
			{
				auto c = e->addComponent<Component::PointLight>();
				c->lightData.colorRange = glm::vec4(
					(float)(std::rand() % 1000) / 1000.f,
					(float)(std::rand() % 1000) / 1000.f,
					(float)(std::rand() % 1000) / 1000.f, 20.0f); // distance
				c->lightData.positionPower.w = 3.f; // intensite
			}
		}
	}

	virtual bool initialize()
	{
		_balls.requireTag(MyTags::BULLET_TAG);
		_balls.requireComponent<Component::Collision>();
		_characterController.requireTag(MyTags::HEROS_TAG);
		_inputs = _scene.lock()->getInstance<Input>();
		if (!_inputs)
			return false;
		return true;
	}

	virtual bool activate()
	{
		return true;
	}

	virtual bool deactivate()
	{
		auto scene = _scene.lock();
		while (_shots.size() > 0)
		{
			scene->destroy(_shots.front());
			_shots.pop();
		}
		return true;
	}
};