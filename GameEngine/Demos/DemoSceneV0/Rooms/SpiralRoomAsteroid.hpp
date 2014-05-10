#pragma once

#include "Room.hpp"
#include <map>
#include <Core/SceneManager.hh>
#include <Systems/SceneInSceneSystem.hpp>

struct SpiralRoomAsteroid : public Room
{
	SpiralRoomAsteroid(std::weak_ptr<AScene> &&scene)
	: Room(std::move(scene))
	, fboReceiver(scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>())
	, fboId(0)
	{}

	virtual ~SpiralRoomAsteroid()
	{}

	virtual bool init()
	{
		// load

		hotZoneSpiralEngine = createHotZone("SpiralAsteroid->Engine", "HZ-engine-asteroid");

		{
			auto &_sid = fboId;
			fboReceiver.globalSub(PubSubKey("fboAsteroidId"), [&_sid](GLuint id) {
				_sid = id;
			});
		}
		return true;
	}

	Entity hotZoneSpiralEngine;
	std::map<std::string, Entity> map;
	//PubSub fboAsteroidReceiver;
	//GLuint fboAsteroidId;
	PubSub fboReceiver;
	GLuint fboId;
protected:
	virtual bool _enable()
	{
		auto scene = _scene.lock();

		for (auto i = 0; i < 2; ++i)
		{
			auto e = scene->createEntity();
			auto l = scene->addComponent<Component::PointLight>(e);
			l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f); // distance
			l->lightData.positionPower.w = 1.0f; // intensite
			scene->addComponent<Component::TransformationRegister>(e, "spiral-asteroid-pointlight-" + std::to_string(i));
			scene->addComponent<Component::EntityPlacable>(e, "spiral-asteroid-pointlight-" + std::to_string(i));
			map["spiral-asteroid-pointlight-" + std::to_string(i)] = e;
		}

		// fbo asteroid
		{
			scene->getInstance<SceneManager>()->enableScene("Asteroid", 2);
			fboReceiver.broadCast(PubSubKey("asteroidPause"));
			auto e = scene->createEntity();
			scene->getSystem<SceneInSceneSystem>()->setScene("asteroid");
			auto &transform = scene->getLocalTransform(e);
			scene->setLocalTransform(e, glm::translate(transform, glm::vec3(-8, 1, 0)));
			auto sprite = scene->addComponent<Component::Sprite>(e, scene->getInstance<SpriteManager>()->getAnimation("FBO-asteroid", "asteroid"));
			sprite->delay = 1.0f / 10.0f;
			sprite->animation->getMaterial().diffuseTex->id = fboId;
			sprite->animation->_frames[0]->_uvs = glm::vec4(1, 0, 0, 1);
			sprite->animation->_frames[0]->load(scene->getInstance < VertexManager<4> >());
			scene->addComponent<Component::TransformationRegister>(e, "fbo-asteroid");
			scene->addComponent<Component::EntityPlacable>(e, "fbo-asteroid");
			map["fbo-asteroid"] = e;
		}
		{
			auto e = scene->createEntity();
			auto sprite = scene->addComponent<Component::Sprite>(e, scene->getInstance<SpriteManager>()->getAnimation("Buttons", "notice_asteroid"));
			scene->addComponent<Component::TransformationRegister>(e, "notice_asteroid");
			scene->addComponent<Component::EntityPlacable>(e, "notice_asteroid");
			map["notice_asteroid"] = e;
		}
		return true;
	}

	virtual bool _disable()
	{
		auto scene = _scene.lock();
		scene->getInstance<SceneManager>()->disableScene("Asteroid");
		scene->getSystem<SceneInSceneSystem>()->setScene("");


		for (auto &e : map)
		{
			scene->destroy(e.second);
		}
		return true;
	}
};