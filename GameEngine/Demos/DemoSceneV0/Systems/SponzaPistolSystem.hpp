#pragma once

#include <Systems/System.h>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>
#include <Components/RigidBody.hpp>
#include <MyTags.hpp>
#include <Components/PointLight.hh>
#include <Components/SpotLight.hh>
#include <Components/CameraComponent.hpp>
#include <Systems/PostFxSystem.hh>


class SponzaPistolSystem : public System
{
public:
	SponzaPistolSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _cameras(std::move(scene))
	{}

	virtual ~SponzaPistolSystem()
	{}

private:
	EntityFilter _cameras;
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

		static std::queue<Entity> stack;

		if (scene->getInstance<Input>()->getInput(SDL_BUTTON_LEFT) && delay <= 0.0f)
		{
			glm::vec3 from, to;
			scene->getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);

			auto e = scene->createEntity();
			e->setLocalTransform(glm::translate(e->getLocalTransform(), (from + to * 1.5f)));
			e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.2f)));
			auto rigidbody = e->addComponent<Component::RigidBody>(10.0f);
			rigidbody->setCollisionShape(Component::RigidBody::SPHERE);
			auto &body = rigidbody->getBody();
			body.applyCentralImpulse(convertGLMVectorToBullet(to * 10.0f));
			body.getBroadphaseHandle()->m_collisionFilterGroup = COLLISION_LAYER_STATIC | COLLISION_LAYER_DYNAMIC;
			body.getBroadphaseHandle()->m_collisionFilterMask = COLLISION_LAYER_DYNAMIC;
			body.setFriction(1.0f);
			body.setRestitution(0.9f);
			auto mesh = e->addComponent<Component::MeshRenderer>(scene->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
			mesh->setShader("MaterialBasic");

			auto light = e->addComponent<Component::PointLight>();
			light->lightData.colorRange = glm::vec4(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, 5.0f);
			light->lightData.positionPower.w = 3.0f;
			e->addTag(MyTags::BULLET_TAG);
			if (stack.size() > 50)
			{
				scene->destroy(stack.front());
				stack.pop();
			}
			stack.push(e);
			delay = 0.1f;
		}

		if (_inputs->getInput(SDL_BUTTON_MIDDLE) && delay <= 0.0f)
		{
			auto e = scene->createEntity();

			auto l = e->addComponent<Component::SpotLight>();
			if (_cameras.getCollection().size() != 0)
			{
				auto cam = _cameras.getCollection().begin()->get()->getComponent<Component::CameraComponent>();
				l->projection = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
				l->lightData.colorRange = glm::vec4(rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, rand() % 10000 / 10000.0f, 100.0f);
				l->lightData.positionPower.w = 50.0f;
				l->lightData.shadowId = 1;
				e->setLocalTransform(glm::inverse(cam->lookAtTransform));
			}
			delay = 0.1f;	
		}
		if (_inputs->getInput(SDLK_h))
			scene->getSystem<PostFxSystem>()->useHDR(true);
		if (_inputs->getInput(SDLK_j))
			scene->getSystem<PostFxSystem>()->useHDR(false);
		if (_inputs->getInput(SDLK_f))
			scene->getSystem<PostFxSystem>()->useBloom(true);
		if (_inputs->getInput(SDLK_g))
			scene->getSystem<PostFxSystem>()->useBloom(false);

		static float	sigma = 5.0f;
		static float	glare = 1.0f;
		if (_inputs->getInput(SDLK_UP))
		{
			sigma += 0.5f;
			scene->getSystem<PostFxSystem>()->setBloomSigma(sigma);
		}
		if (_inputs->getInput(SDLK_DOWN))
		{
			sigma -= 0.5f;
			scene->getSystem<PostFxSystem>()->setBloomSigma(sigma);
		}
		if (_inputs->getInput(SDLK_LEFT))
		{
			glare -= 0.2f;
			scene->getSystem<PostFxSystem>()->setBloomGlare(glare);
		}
		if (_inputs->getInput(SDLK_RIGHT))
		{
			glare += 0.2f;
			scene->getSystem<PostFxSystem>()->setBloomGlare(glare);
		}
		if (delay >= 0.0f)
			delay -= static_cast<float>(time);
	}

	virtual bool initialize()
	{
		_inputs = _scene.lock()->getInstance<Input>();
		_cameras.requireComponent<Component::CameraComponent>();
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