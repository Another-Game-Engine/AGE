#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/HotZone.hpp>
#include <Components/Collision.hpp>
#include <MyTags.hpp>
#include <Rooms/Room.hpp>
#include <Context/SdlContext.hh>
#include <Components/TransformationRegister.hpp>
#include <Components/RotationForce.hpp>
#include <Components/AudioEmitter.hpp>
#include <Components/PointLight.hh>
#include <map>

class DemoCatSystem : public System
{
public:
	DemoCatSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
	{}

	virtual ~DemoCatSystem()
	{}

private:
	std::shared_ptr<Input> _inputs;
	std::map<std::string, Entity> _map;
	std::map<std::string, bool> _cpts;
	glm::mat4 _pos;
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		static float t = 0.0f;
		float ftime = (float)time;
		static unsigned int i = 0;
		if (t > 5.5f)
		{
			auto it = _cpts.begin();
			std::advance(it, i);
			auto key = it->first;
			auto value = it->second;

			if (!value)
			{
				auto scene = _scene.lock();

				if (key == "Point Light")
				{
					auto c = _map["cat"]->addComponent<Component::PointLight>();
					c->lightData.colorRange = glm::vec4(
						(float)(std::rand() % 1000) / 1000.f,
						(float)(std::rand() % 1000) / 1000.f,
						(float)(std::rand() % 1000) / 1000.f, 20.0f); // distance
					c->lightData.positionPower.w = 3.f; // intensite
					_map["pointlight-button"]->getComponent<Component::Sprite>()->animation = scene->getInstance<SpriteManager>()->getAnimation("Buttons", "pointlight_on");
				}
				else if (key == "Rigid Body")
				{
					auto c = _map["cat"]->addComponent<Component::RigidBody>(5.0f);
					c->setCollisionShape(Component::RigidBody::MESH, "collision_shape_dynamic_starship");
					_map["rigidbody-button"]->getComponent<Component::Sprite>()->animation = scene->getInstance<SpriteManager>()->getAnimation("Buttons", "rigidbody_on");
				}
				else if (key == "Sound Emitter")
				{
					auto audioCpt = _map["cat"]->addComponent<Component::AudioEmitter>();
					audioCpt->setAudio(scene->getInstance<AudioManager>()->getAudio("space-sound"), "space-sound", CHANNEL_GROUP_MUSIC);
					audioCpt->play("space-sound", false);
					audioCpt->getAudio("space-sound")->channel->set3DMinMaxDistance(0.01f, 0.02f);
					_map["soundemitter-button"]->getComponent<Component::Sprite>()->animation = scene->getInstance<SpriteManager>()->getAnimation("Buttons", "soundemitter_on");

				}
			}
			else
			{
				auto scene = _scene.lock();
				auto cat = _map["cat"];
				if (key == "Point Light")
				{
					cat->removeComponent<Component::PointLight>();
					_map["pointlight-button"]->getComponent<Component::Sprite>()->animation = scene->getInstance<SpriteManager>()->getAnimation("Buttons", "pointlight_off");
				}
				else if (key == "Rigid Body")
				{
					cat->removeComponent<Component::RigidBody>();
					cat->setLocalTransform(_pos);
					_map["rigidbody-button"]->getComponent<Component::Sprite>()->animation = scene->getInstance<SpriteManager>()->getAnimation("Buttons", "rigidbody_off");
				}
				else if (key == "Sound Emitter")
				{
					cat->removeComponent<Component::AudioEmitter>();
					_map["soundemitter-button"]->getComponent<Component::Sprite>()->animation = scene->getInstance<SpriteManager>()->getAnimation("Buttons", "soundemitter_off");
				}
			}

			it->second = !it->second;
			t = 0.0f;
			++i;
			if (i >= _cpts.size())
				i = 0;
		}

		t += ftime;
	}

	virtual bool initialize()
	{
		_inputs = _scene.lock()->getInstance<Input>();
		if (!_inputs)
			return false;
		_scene.lock()->getInstance<AudioManager>()->loadStream(File("../../Assets/Sounds/space-sound.mp3"), Audio::AudioSpatialType::AUDIO_3D, "space-sound");
		_scene.lock()->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__starship.cpd"));
		if (!_scene.lock()->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Buttons.CPDAnimation")))
			return false;

		return true;
	}

	virtual bool activate()
	{
		auto scene = _scene.lock();
		_cpts["Point Light"] = false;
		_cpts["Rigid Body"] = false;
		_cpts["Sound Emitter"] = false;


		// CAT
		{
			auto e = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__starship");
			if (!meshObj)
				return false;
			auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			e->addComponent<Component::EntityPlacable>("engine-room-gros-chat");
			e->addComponent<Component::TransformationRegister>("engine-room-gros-chat");
			_pos = e->getLocalTransform();
			_map["cat"] = e;
		}

		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Buttons", "soundemitter_off"));
			e->addComponent<Component::TransformationRegister>("soundemitter-button");
			e->addComponent<Component::EntityPlacable>("soundemitter-button");
			_map["soundemitter-button"] = e;
		}
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Buttons", "pointlight_off"));
			e->addComponent<Component::TransformationRegister>("pointlight-button");
			e->addComponent<Component::EntityPlacable>("pointlight-button");
			_map["pointlight-button"] = e;
		}
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Buttons", "rigidbody_off"));
			e->addComponent<Component::TransformationRegister>("rigidbody-button");
			e->addComponent<Component::EntityPlacable>("rigidbody-button");
			_map["rigidbody-button"] = e;
		}
		{
			auto e = scene->createEntity();
			auto rb = e->addComponent<Component::RigidBody>(0.0f);
			rb->setCollisionShape(Component::RigidBody::BOX, "NULL");
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
			if (!meshObj)
				return false;
			auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			e->addComponent<Component::TransformationRegister>("otel-button");
			e->addComponent<Component::EntityPlacable>("otel-button");
			_map["otel-button"] = e;
		}
		return true;
	}

	virtual bool deactivate()
	{
		auto scene = _scene.lock();
		for (auto &e : _map)
			scene->destroy(e.second);
		_map.clear();
		_cpts.clear();
		return true;
	}
};