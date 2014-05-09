#include "PhysicsRoom.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Systems/HotZoneSystem.hpp>
#include <Components/CollisionBody.hpp>
#include <Components/SpotLight.hh>
#include <Components/AudioEmitter.hpp>

class PistolSystem;		

	PhysicsRoom::PhysicsRoom(std::weak_ptr<AScene> &&scene)
		: Room(std::move(scene))
	{
	}

	PhysicsRoom::~PhysicsRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool PhysicsRoom::init()
	{
		// load
		auto s = _scene.lock();
		s->getInstance<AudioManager>()->loadStream(File("../../Assets/Sounds/the-wall.mp3"), Audio::AudioSpatialType::AUDIO_3D, "the-wall");
		hotZonePhysicsProjection = createHotZone("Physics->Projection", "HZ-projection-physics");
		return true;
	}

	bool PhysicsRoom::_enable()
	{
		auto scene = _scene.lock();
		scene->activateSystem<PistolSystem>();

		{
			glm::mat4 t(1);
			t = glm::translate(t, glm::vec3(-14.5, -0.1f, -34.0f));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
			auto w = 10;
			auto h = 6;
			for (auto i = 0; i < h; i++)
			{
				for (auto j = 0; j < w; ++j)
				{
						auto e = scene->createEntity();
						auto &trans = scene->getGlobalTransform(e);
						trans = t;
						trans = glm::scale(t, glm::vec3(0.6f));
						trans = glm::translate(trans, glm::vec3(0, h - i + 0.001f, ((float)(w)-(float)(j)) + 4.0f));
						scene->addComponent<Component::MeshRenderer>(e, meshObj)->setShader("MaterialBasic");
						scene->addComponent<Component::RigidBody>(e, _scene, 1.0f)->setCollisionShape(e, Component::RigidBody::BOX);
						cubes.push_back(e);
				}
			}
		}
		{
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");

			for (auto i = 0; i < 3; ++i)
			{
				auto light = scene->createEntity();
				auto l = scene->addComponent<Component::SpotLight>(light);
				l->lightData.colorRange = glm::vec4(0.4f, 0.6f, 1.0f, 100.0f);
				l->lightData.positionPower.w = 10.0f;
				l->projection = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
				l->lightData.shadowId = 1;
				scene->addComponent<Component::EntityPlacable>(light, "physics-room-spotlight-" + std::to_string(i));
				scene->addComponent<Component::TransformationRegister>(light, "physics-room-spotlight-" + std::to_string(i));
				lights.push_back(light);
			}
			{
				auto audioCpt = scene->addComponent<Component::AudioEmitter>(*lights.begin());
				audioCpt->setAudio(scene->getInstance<AudioManager>()->getAudio("the-wall"), "the-wall", CHANNEL_GROUP_MUSIC);
				audioCpt->play("the-wall", false);
				audioCpt->getAudio("the-wall")->channel->set3DMinMaxDistance(1.0f, 30.0f);
			}
			for (auto i = 3; i < 5; ++i)
			{
				auto light = scene->createEntity();
				auto l = scene->addComponent<Component::PointLight>(light);
				l->lightData.colorRange = glm::vec4(0.4f, 0.6f, 1.0f, 6.0f); // distance
				l->lightData.positionPower.w = 3.0f; // intensite
				scene->addComponent<Component::TransformationRegister>(light, "physics-room-pointlight-" + std::to_string(i));
				scene->addComponent<Component::EntityPlacable>(light, "physics-room-pointlight-" + std::to_string(i));
				lights.push_back(light);
			}
			scene->getComponent<Component::PointLight>(lights[3])->lightData.colorRange = glm::vec4(0.6f, 0.6f, 0.2f, 16.0f);
			scene->getComponent<Component::PointLight>(lights[4])->lightData.colorRange = glm::vec4(0.6f, 0.9f, 0.2f, 5.0f);

		}
		{
			clickToShoot = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__click-to-shoot");
			if (!meshObj)
				return false;
			auto meshComponent = scene->addComponent<Component::MeshRenderer>(clickToShoot, meshObj);
			meshComponent->setShader("MaterialBasic");
			scene->addComponent<Component::TransformationRegister>(clickToShoot, "click-to-shoot-text");
			scene->addComponent<Component::EntityPlacable>(clickToShoot, "click-to-shoot-text");
		}
		return true;
	}

	bool PhysicsRoom::_disable()
	{
		auto scene = _scene.lock();
		for (auto e : cubes)
		{
			scene->destroy(e);
		}
		for (auto e : lights)
		{
			scene->destroy(e);
		}
		cubes.clear();
		lights.clear();
		scene->deactivateSystem<PistolSystem>();
		scene->destroy(clickToShoot);
		return true;
	}