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
		: _scene(o._scene)
		, _projection(1)
		, _key(o._key)
	{
	}

	CameraComponent	&CameraComponent::operator=(CameraComponent const &o)
	{
		_scene = o._scene;
		_key = o._key;
		_projection = o._projection;
		return *this;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_projection = projection;
		_scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_key, _projection);
	}

	const glm::mat4 &CameraComponent::getProjection() const
	{
		return _projection;
	}

	void CameraComponent::init(AScene *scene)
	{
		_scene = scene;
		_key = scene->getInstance<AGE::Threads::Prepare>()->addCamera();
		scene->getLink(entityId)->registerOctreeObject(_key);
		setProjection(glm::perspective(60.0f, 1600.0f / 900.0f, 0.1f, 2000.0f));
	}

	void CameraComponent::reset(AScene *scene)
	{
		assert(!_key.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_key);
		_projection = glm::mat4(1);
	}

	void CameraComponent::postUnserialization(AScene *scene)
	{
		init(scene);
		setProjection(_projection);
	}

};

