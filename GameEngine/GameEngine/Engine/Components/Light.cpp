#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Render/GeometryManager.hh>
#include <Core/PrepareRenderThread.hpp>

namespace Component
{
	PointLight::PointLight()
	{
	}

	PointLight::~PointLight()
	{

	}

	PointLight::PointLight(PointLight const &o)
		: _scene(o._scene),
		_key(o._key)
	{

	}

	PointLight &PointLight::operator=(PointLight const &p)
	{
		_scene = p._scene;
		_key = p._key;
		return (*this);
	}

	void PointLight::reset(AScene *)
	{
		assert(!_key.invalid());
		_scene->getLink(entityId)->unregisterOctreeObject(_key);
		_scene->getInstance<AGE::Threads::Prepare>()->removeElement(_key);
		_key = AGE::PrepareKey();
	}

	void PointLight::init(AScene *scene)
	{
		_scene = scene;
		_key = scene->getInstance<AGE::Threads::Prepare>()->addPointLight();
		scene->getLink(entityId)->registerOctreeObject(_key);
		assert(!_key.invalid());
	}

	PointLight &PointLight::setPosition(glm::vec4 const &position)
	{
		_scene->getInstance<AGE::Threads::Prepare>()->setPosition(glm::vec3(position.x, position.y, position.z), _key);
		return (*this);
	}

	PointLight &PointLight::set(glm::vec3 const &position, glm::vec3 const &color, glm::vec3 const &range)
	{
		_scene->getInstance<AGE::Threads::Prepare>()->setPointLight(position, color, range, _key);
		return (*this);
	}
}