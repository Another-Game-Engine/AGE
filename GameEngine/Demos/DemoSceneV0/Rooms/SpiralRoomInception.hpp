#pragma once

#include "Room.hpp"
#include <map>
#include <Core/SceneManager.hh>
#include <Systems/SceneInSceneSystem.hpp>
#include <Scenes/MainScene.hh>

struct SpiralRoomInception : public Room
{
	SpiralRoomInception(std::weak_ptr<AScene> scene)
	: Room(scene)
	, fboReceiver(scene.lock()->getInstance<PubSub::Manager>())
	, fboId(0)
	{}

	virtual ~SpiralRoomInception()
	{}

	virtual bool init()
	{
		// load

		hotZoneSpiralEngine = createHotZone("SpiralInception->Engine", "HZ-engine-inception");
		{
			auto &_sid = fboId;
			fboReceiver.globalSub(PubSubKey("fboInceptionId"), [&_sid](GLuint id) {
				_sid = id;
			});
		}
		return true;
	}

	Entity hotZoneSpiralEngine;
	std::map<std::string, Entity> map;
	PubSub fboReceiver;
	GLuint fboId;
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
			e->addComponent<Component::TransformationRegister>("spiral-inception-pointlight-" + std::to_string(i));
			e->addComponent<Component::EntityPlacable>("spiral-inception-pointlight-" + std::to_string(i));
			map["spiral-inception-pointlight-" + std::to_string(i)] = e;
		}

		// fbo scene next
		{
			auto e = scene->createEntity();
//scene->getSystem<SceneInSceneSystem>()->setScene("inception");
			e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(-8, 1, 0)));
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("FBO-inception", "inception"));
			sprite->delay = 1.0f / 10.0f;
			sprite->animation->getMaterial().diffuseTex->id = fboId;
			sprite->animation->_frames[0]->_uvs = glm::vec4(1,1,0,0);
			sprite->animation->_frames[0]->load(scene->getInstance < VertexManager<4> >());
			e->addComponent<Component::TransformationRegister>("fbo-inception");
			e->addComponent<Component::EntityPlacable>("fbo-inception");
			map["fbo-inception"] = e;
		}
		return true;
	}

	virtual bool _disable()
	{
		auto scene = _scene.lock();

		for (auto &e : map)
		{
			scene->destroy(e.second);
		}
		return true;
	}
};