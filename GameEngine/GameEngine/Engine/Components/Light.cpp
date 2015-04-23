#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#ifdef EDITOR_ENABLED
#include <imgui\imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	PointLightComponent::PointLightComponent()
		: _range(1.0f, 0.01f, 0.001f)
		, _color(1),
		_map(nullptr)
	{
	}

	PointLightComponent::~PointLightComponent()
	{

	}

	PointLightComponent::PointLightComponent(PointLightComponent const &o)
		: _key(o._key)
		, _range(o._range)
		, _color(o._color),
		_map(o._map)
	{
		postUnserialization();
	}

	void PointLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const PointLightComponent*>(model);
		_range = o->_range;
		_color = o->_color;
		postUnserialization();
	}

	void PointLightComponent::reset()
	{
		if (!_key.invalid())
		{
			entity.getLink().unregisterOctreeObject(_key);
		}
		_key = AGE::PrepareKey();
		_color = glm::vec3(1);
		_range = glm::vec3(1.0f, 0.01f, 0.001f);
		set(_color, _range);
	}

	void PointLightComponent::init()
	{
		_key = AGE::GetPrepareThread()->addPointLight();
		entity.getLink().registerOctreeObject(_key);
		_map = entity.getScene()->getInstance<AssetsManager>()->getPointLightTexture();
		assert(!_key.invalid());
		set(_color, _range);
	}

	PointLightComponent &PointLightComponent::set(glm::vec3 const &color, glm::vec3 const &range)
	{
		_color = color;
		_range = range;
		AGE::GetPrepareThread()->setPointLight(color, range, _map, _key);
		return (*this);
	}

	float		PointLightComponent::computePointLightRange(float minValue, glm::vec3 const &attenuation)
	{
		glm::vec3 equation(attenuation.z, attenuation.y, attenuation.x - minValue);
		float discriminant = Mathematic::secondDegreeDiscriminant(equation);
		if (discriminant == 0)
		{
			return (Mathematic::resolveSecondDegree(equation));
		}
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


	void PointLightComponent::postUnserialization()
	{
		init();
		set(_color, _range);
	}

#ifdef EDITOR_ENABLED
	void PointLightComponent::editorCreate()
	{}

	void PointLightComponent::editorDelete()
	{}

	bool PointLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", getColorPtr()))
		{
			set(_color, _range);
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(_range), 0.0f, 1.0f))
		{
			set(_color, _range);
			modified = true;
		}
		return modified;
	}
#endif
}