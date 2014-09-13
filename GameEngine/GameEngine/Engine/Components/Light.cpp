#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Render/GeometryManager.hh>

namespace Component
{
	PointLight::PointLight()
		: _power(1.0f),
		_range(1.0f),
		_color(1.0f),
		_position(1.0f)
	{
	}

	PointLight::~PointLight()
	{

	}

	PointLight::PointLight(PointLight const &o)
		: _power(o._power),
		_range(o._range),
		_color(o._color),
		_position(o._position)
	{

	}

	PointLight &PointLight::operator=(PointLight const &p)
	{
		_power = p._power;
		_range = p._range;
		_position = p._position;
		_color = p._color;
		return (*this);
	}

	void PointLight::reset(AScene *)
	{

	}

	void PointLight::init(AScene *)
	{

	}

	AGE::PrepareElement &PointLight::initOctree(AScene *scene, ENTITY_ID entityId)
	{
		_scene = scene;
		_OTKey = scene->getInstance<AGE::Threads::Prepare>()->addPointLightElement();
		scene->getLink(entityId)->registerOctreeObject(_OTKey);
		assert(!_OTKey.invalid());
		return (*this);
	}

	AGE::PrepareElement &PointLight::resetOctree(AScene *scene, ENTITY_ID entityId)
	{
		assert(!_OTKey.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_OTKey);
		scene->getInstance<AGE::Threads::Prepare>()->removeElement(_OTKey);
		_OTKey = AGE::PrepareKey();
		return (*this);
	}

	PointLight &PointLight::setPosition(glm::vec4 const &position)
	{
		_position = position;
		_scene->getInstance<AGE::Threads::Prepare>()->setPosition(glm::vec3(_position.x, _position.y, _position.z), _OTKey);
		return (*this);
	}

	PointLight &PointLight::set(float power, float range, glm::vec3 const &color, glm::vec4 const &position)
	{
		_power = power;
		_range = range;
		_color = color;
		_position = position;
		_scene->getInstance<AGE::Threads::Prepare>()->setPointLight(power, range, color, position, _OTKey);
		return (*this);
	}
}