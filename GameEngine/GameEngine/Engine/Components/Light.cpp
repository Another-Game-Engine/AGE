#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Render/GeometryManager.hh>

namespace Component
{
	PointLight::PointLight()
		: power(1.0f),
		range(1.0f),
		color(1.0f),
		position(1.0f)
	{
	}

	PointLight::~PointLight()
	{

	}

	PointLight::PointLight(PointLight const &o)
		: power(o.power),
		range(o.range),
		color(o.color),
		position(o.position)
	{

	}

	PointLight &PointLight::operator=(PointLight const &p)
	{
		power = p.power;
		range = p.range;
		position = p.position;
		color = p.color;
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
	//	_OTKey = scene->getInstance<AGE::Threads::Prepare>()->addLightElement();
		scene->getLink(entityId)->registerOctreeObject(_OTKey);
		assert(!_OTKey.invalid());
		return (*this);
	}

	AGE::PrepareElement &PointLight::resetOctree(AScene *scene, ENTITY_ID entityId)
	{
		assert(!_OTKey.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_OTKey);
		//scene->getInstance<AGE::Threads::Prepare>()->removeElement(_OTKey);
		_OTKey = AGE::PrepareKey();
		return (*this);
	}
}