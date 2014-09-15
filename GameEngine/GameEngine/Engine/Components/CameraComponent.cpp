#include <Components/CameraComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <Core/PrepareRenderThread.hpp>

namespace Component
{
	CameraComponent::CameraComponent()
		: ComponentBase<CameraComponent>()
	{
	}

	CameraComponent::~CameraComponent(void)
	{
	}

	CameraComponent::CameraComponent(CameraComponent const &o)
		: _scene(o._scene),
		_key(o._key)
	{
	}

	CameraComponent	&CameraComponent::operator=(CameraComponent const &o)
	{
		_scene = o._scene;
		_key = o._key;
		return *this;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_key, projection);
	}

	const glm::mat4 &CameraComponent::getProjection() const
	{
		if (_scene == nullptr)
			assert(0);
		return (_scene->getInstance<AGE::Threads::Prepare>()->getProjection(_key));
	}

	void CameraComponent::init(AScene *scene)
	{
		_scene = scene;
		_key = scene->getInstance<AGE::Threads::Prepare>()->addCamera();
		scene->getLink(entityId)->registerOctreeObject(_key);
		scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_key, glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f));
	}

	void CameraComponent::reset(AScene *scene)
	{
		if (!_key.invalid())
			assert(0);
		scene->getLink(entityId)->unregisterOctreeObject(_key);
		scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
	}

};

