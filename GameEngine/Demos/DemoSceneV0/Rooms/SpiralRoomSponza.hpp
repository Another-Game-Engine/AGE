#pragma once

#include "Room.hpp"
#include <map>
#include <Core/SceneManager.hh>
#include <Systems/SceneInSceneSystem.hpp>

struct SpiralRoomSponza : public Room
{
	SpiralRoomSponza(std::weak_ptr<AScene> scene)
	: Room(scene)
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
			auto l = e->addComponent<Component::PointLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f); // distance
			l->lightData.positionPower.w = 1.0f; // intensite
			e->addComponent<Component::TransformationRegister>("spiral-sponza-pointlight-" + std::to_string(i));
			e->addComponent<Component::EntityPlacable>("spiral-sponza-pointlight-" + std::to_string(i));
			map["spiral-sponza-pointlight-" + std::to_string(i)] = e;
		}

		// fbo sponza
		{
			scene->getInstance<SceneManager>()->enableScene("Sponza", 2);
			fboSponzaReceiver.broadCast(PubSubKey("sponzaPause"));
			auto e = scene->createEntity();
			scene->getSystem<SceneInSceneSystem>()->setScene("sponza");
			e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(-8, 1, 0)));
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("FBO-sponza", "sponza"));
			sprite->delay = 1.0f / 10.0f;
			sprite->animation->getMaterial().diffuseTex->id = fboSponzaId;
			sprite->animation->_frames[0]->_uvs = glm::vec4(1, 0, 0, 1);
			sprite->animation->_frames[0]->load(scene->getInstance < VertexManager<4> >());
			e->addComponent<Component::TransformationRegister>("fbo-sponza");
			e->addComponent<Component::EntityPlacable>("fbo-sponza");
			map["fbo-sponza"] = e;
		}
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Buttons", "notice_sponza"));
			e->addComponent<Component::TransformationRegister>("notice_sponza");
			e->addComponent<Component::EntityPlacable>("notice_sponza");
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