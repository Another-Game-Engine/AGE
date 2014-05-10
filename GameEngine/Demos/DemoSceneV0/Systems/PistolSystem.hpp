#pragma once

#include <Systems/System.h>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>
#include <Components/RigidBody.hpp>
#include <MyTags.hpp>

class PistolSystem : public System
{
public:
	PistolSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _characterController(std::move(scene))
	{}

	virtual ~PistolSystem()
	{}

private:
	EntityFilter _characterController;
	std::queue<Entity> _shots;
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
		if (_inputs->getInput(SDL_BUTTON_LEFT) && delay <= 0.0f)
		{
			for (auto controller : _characterController.getCollection())
			{
				glm::vec3 from, to;
				scene->getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
				auto e = scene->createEntity();
				auto trans = scene->getLocalTransform(e);
				trans = glm::translate(trans, glm::vec3(from + to * 1.2f));
				trans = glm::scale(trans, glm::vec3(0.23f));
				scene->setLocalTransform(e, trans);
				auto mesh = scene->addComponent<Component::MeshRenderer>(e, scene->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
				mesh->setShader("MaterialBasic");
				auto rigidBody = scene->addComponent<Component::RigidBody>(e, _scene, 0.4f);
				rigidBody->setCollisionShape(e, Component::RigidBody::SPHERE);
				rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(to * 8.0f));
				rigidBody->getBody().setFriction(1.0f);
				rigidBody->getBody().setRestitution(1.0f);
				scene->addTag(e, MyTags::BULLET_TAG);
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
	}

	virtual bool initialize()
	{
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