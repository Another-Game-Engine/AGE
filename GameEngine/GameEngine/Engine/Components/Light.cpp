#include <Components/Light.hh>
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

	AGE::OctreeElement &PointLight::initOctree(AScene *scene, ENTITY_ID entityId)
	{
		return (*this);
	}

	AGE::OctreeElement &PointLight::resetOctree(AScene *scene, ENTITY_ID entityId)
	{
		return (*this);
	}
}