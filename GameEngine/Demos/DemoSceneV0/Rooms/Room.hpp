#pragma once

#include <memory>
#include <Core/AScene.hh>
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Components/CollisionBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Components/EntityPlacable.hpp>
#include <Components/TransformationRegister.hpp>
#include <Components/PointLight.hh>
#include <Components/HotZone.hpp>

struct Room : public std::enable_shared_from_this<Room>
{
	Room(std::weak_ptr<AScene> scene)
		: _scene(scene)
	{
	}

	virtual ~Room()
	{}

	virtual bool init() = 0;

	inline bool isEnable() const { return _enabled; }
	inline bool enable()
	{
		if (_enabled)
			return false;
		_enabled = true;
		return _enable();
	}

	inline bool disable()
	{
		if (!_enabled)
			return false;
		_enabled = false;
		return _disable();
	}
protected:
	std::weak_ptr<AScene> _scene;
	virtual bool _enable() = 0;
	virtual bool _disable() = 0;
	Entity createHotZone(const std::string &name, const std::string &sharedName)
	{
		auto scene = _scene.lock();
		auto hotZone = scene->createEntity();
		auto rb = hotZone->addComponent<Component::RigidBody>(0.0f);
		rb->setCollisionShape(Component::RigidBody::BOX, "NULL");
		hotZone->setLocalTransform(glm::scale(hotZone->getLocalTransform(), glm::vec3(1, 0.1, 1)));
		auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
		if (!meshObj)
			return false;
		//auto meshComponent = hotZone->addComponent<Component::MeshRenderer>(meshObj);
		//meshComponent->setShader("MaterialBasic");
		hotZone->addComponent<Component::HotZone>(name, sharedName, shared_from_this());
		//hotZone->addComponent<Component::EntityPlacable>(name);
		hotZone->addComponent<Component::TransformationRegister>(name);
		return hotZone;
	}
private:
	bool _enabled = false;
};