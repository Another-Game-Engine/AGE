#pragma once

#include "Room.hpp"
#include <map>
#include <Core/SceneManager.hh>
#include <Systems/SceneInSceneSystem.hpp>

struct SpiralRoomSponza : public Room
{
	SpiralRoomSponza(std::weak_ptr<AScene> &&scene)
	: Room(std::move(scene))
	, fboSponzaReceiver(scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>())
	, fboSponzaId(0)
	{}

	virtual ~SpiralRoomSponza()
	{}

	virtual bool init()
	{
		// load

		hotZoneSpiralEngine = createHotZone("SpiralSponza->Engine", "HZ-engine-sponza");

		{
			auto &_sid = fboSponzaId;
			fboSponzaReceiver.globalSub(PubSubKey("fboSponzaId"), [&_sid](GLuint id) {
				_sid = id;
			});
		}
		return true;
	}

	Entity hotZoneSpiralEngine;
	std::map<std::string, Entity> map;
	//PubSub fboAsteroidReceiver;
	//GLuint fboAsteroidId;
	PubSub fboSponzaReceiver;
	GLuint fboSponzaId;
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
			scene->addComponent<Component::TransformationRegister>(e, "spiral-sponza-pointlight-" + std::to_string(i));
			scene->addComponent<Component::EntityPlacable>(e, "spiral-sponza-pointlight-" + std::to_string(i));
			map["spiral-sponza-pointlight-" + std::to_string(i)] = e;
		}

		// fbo sponza
		{
			scene->getInstance<SceneManager>()->enableScene("Sponza", 2);
			fboSponzaReceiver.broadCast(PubSubKey("sponzaPause"));
			auto e = scene->createEntity();
			scene->getSystem<SceneInSceneSystem>()->setScene("sponza");
			auto &transform = scene->getLocalTransform(e);
			scene->setLocalTransform(e, glm::translate(transform, glm::vec3(-8, 1, 0)));
			auto sprite = scene->addComponent<Component::Sprite>(e, scene->getInstance<SpriteManager>()->getAnimation("FBO-sponza", "sponza"));
			sprite->delay = 1.0f / 10.0f;
			sprite->animation->getMaterial().diffuseTex->id = fboSponzaId;
			sprite->animation->_frames[0]->_uvs = glm::vec4(1, 0, 0, 1);
			sprite->animation->_frames[0]->load(scene->getInstance < VertexManager<4> >());
			scene->addComponent<Component::TransformationRegister>(e, "fbo-sponza");
			scene->addComponent<Component::EntityPlacable>(e, "fbo-sponza");
			map["fbo-sponza"] = e;
		}
		{
			auto e = scene->createEntity();
			auto sprite = scene->addComponent<Component::Sprite>(e, scene->getInstance<SpriteManager>()->getAnimation("Buttons", "notice_sponza"));
			scene->addComponent<Component::TransformationRegister>(e, "notice_sponza");
			scene->addComponent<Component::EntityPlacable>(e, "notice_sponza");
			map["notice_sponza"] = e;
		}
		return true;
	}

	virtual bool _disable()
	{
		auto scene = _scene.lock();
		scene->getInstance<SceneManager>()->disableScene("Sponza");
		scene->getSystem<SceneInSceneSystem>()->setScene("");


		for (auto &e : map)
		{
			scene->destroy(e.second);
		}
		return true;
	}
};