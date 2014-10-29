#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Core/PrepareRenderThread.hpp>

#include <Utils/MathematicTools.hh>

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

	void PointLight::reset(AScene *scene)
	{
		assert(!_key.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_key);
		_key = AGE::PrepareKey();
	}

	void PointLight::init(AScene *scene)
	{
		_scene = scene;
		_key = scene->getInstance<AGE::Threads::Prepare>()->addPointLight();
		scene->getLink(entityId)->registerOctreeObject(_key);
		assert(!_key.invalid());
	}

	PointLight &PointLight::set(glm::vec3 const &color, glm::vec3 const &range)
	{
		float	maxRange = computePointLightRange(256, range);

		_scene->getInstance<AGE::Threads::Prepare>()->setPointLight(color, range, _key);
		return (*this);
	}

	float		PointLight::computePointLightRange(float minValue, glm::vec3 const &attenuation)
	{
		glm::vec3 equation(attenuation.z, attenuation.y, attenuation.x - minValue);
		float discriminant = Mathematic::secondDegreeDiscriminant(equation);
		if (discriminant == 0)
			return (Mathematic::resolveSecondDegree(equation));
		else if (discriminant > 0)
		{
			glm::vec2	results = Mathematic::resolveSecondDegree(equation, discriminant);
			return (glm::max(results.x, results.y));
		}
		else
		{
			assert(!"The impossible has happenned :/");
			return (0);
		}
	}

}