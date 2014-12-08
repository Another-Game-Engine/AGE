#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Core/PrepareRenderThread.hpp>

#include <Utils/MathematicTools.hh>

#include <Threads/PrepareRenderThread.hpp>
#include <Threads/ThreadManager.hpp>

namespace Component
{
	PointLight::PointLight()
		: _scene(nullptr)
		, _range(1)
		, _color(1)
	{
	}

	PointLight::~PointLight()
	{

	}

	PointLight::PointLight(PointLight const &o)
		: _scene(o._scene)
		, _key(o._key)
		, _range(o._range)
		, _color(o._color)
	{

	}

	PointLight &PointLight::operator=(PointLight const &p)
	{
		_scene = p._scene;
		_key = p._key;
		_range = p._range;
		_color = p._color;
		return (*this);
	}

	void PointLight::reset(AScene *scene)
	{
		assert(!_key.invalid());
		scene->getLink(entityId)->unregisterOctreeObject(_key);
		_key = AGE::PrepareKey();
		_color = glm::vec3(1);
		_range = glm::vec3(1);
	}

	void PointLight::init(AScene *scene)
	{
		_scene = scene;
		_key = AGE::GetPrepareThread()->addPointLight();
		scene->getLink(entityId)->registerOctreeObject(_key);
		assert(!_key.invalid());
	}

	PointLight &PointLight::set(glm::vec3 const &color, glm::vec3 const &range)
	{
		float	maxRange = computePointLightRange(256, range);
		_color = color;
		_range = range;
		AGE::GetPrepareThread()->setPointLight(color, range, _key);
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


	void PointLight::postUnserialization(AScene *scene)
	{
		init(scene);
		set(_color, _range);
	}
}