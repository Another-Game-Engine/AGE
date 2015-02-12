#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <glm/glm.hpp>

#ifdef EDITOR_ENABLED
#include <imgui\imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	PointLightComponent::PointLightComponent()
		: _scene(nullptr)
		, _range(1)
		, _color(1)
	{
	}

	PointLightComponent::~PointLightComponent()
	{

	}

	PointLightComponent::PointLightComponent(PointLightComponent const &o)
		: _scene(o._scene)
		, _key(o._key)
		, _range(o._range)
		, _color(o._color)
	{

	}

	PointLightComponent &PointLightComponent::operator=(PointLightComponent const &p)
	{
		_scene = p._scene;
		_key = p._key;
		_range = p._range;
		_color = p._color;
		return (*this);
	}

	void PointLightComponent::reset(AScene *scene)
	{
		if (!_key.invalid())
			entity.getLink().unregisterOctreeObject(_key);
		_key = AGE::PrepareKey();
		_color = glm::vec3(1);
		_range = glm::vec3(1);
	}

	void PointLightComponent::init(AScene *scene)
	{
		_scene = scene;
		_key = AGE::GetPrepareThread()->addPointLight();
		entity.getLink().registerOctreeObject(_key);
		assert(!_key.invalid());
	}

	PointLightComponent &PointLightComponent::set(glm::vec3 const &color, glm::vec3 const &range)
	{
		float	maxRange = computePointLightRange(256, range);
		_color = color;
		_range = range;
		AGE::GetPrepareThread()->setPointLight(color, range, _key);
		return (*this);
	}

	float		PointLightComponent::computePointLightRange(float minValue, glm::vec3 const &attenuation)
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


	void PointLightComponent::postUnserialization(AScene *scene)
	{
		init(scene);
		set(_color, _range);
	}

	void PointLightComponent::editorCreate(AScene *scene)
	{}

	void PointLightComponent::editorDelete(AScene *scene)
	{}

	void PointLightComponent::editorUpdate(AScene *scene)
	{
		if (ImGui::SliderFloat3("Color", glm::value_ptr(_color), 0.0f, 1.0f))
		{
			set(_color, _range);
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(_range), 0.0f, 1000.0f))
		{
			set(_color, _range);
		}
	}
}