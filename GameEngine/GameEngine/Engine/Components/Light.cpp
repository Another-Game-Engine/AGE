#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <Threads/ThreadManager.hpp>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#ifdef EDITOR_ENABLED
#include <imgui\imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	
	PointLightData::PointLightData(glm::vec3 const &color, glm::vec3 const &range, std::shared_ptr<ITexture> const &map)
		: color(color),
		range(range),
		map(map)
	{

	}

	PointLightComponent::PointLightComponent(PointLightComponent const &o)
		: _key(o._key)
		, _data(o._data)
	{
		postUnserialization();
	}

	void PointLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const PointLightComponent*>(model);
		_data.range = o->_data.range;
		_data.color = o->_data.color;
		postUnserialization();
	}

	void PointLightComponent::reset()
	{
		if (!_key.invalid())
		{
			//entity->getLink().unregisterOctreeObject(_key);
		}
		_key = AGE::PrepareKey();
		_data.color = glm::vec3(1);
		_data.range = glm::vec3(1.0f, 0.01f, 0.001f);
		_data.map = nullptr;
	}

	void PointLightComponent::init()
	{
		//_key = AGE::GetPrepareThread()->addPointLight();
		//entity->getLink().registerOctreeObject(_key);
		_data.map = entity->getScene()->getInstance<AssetsManager>()->getPointLightTexture();
		assert(!_key.invalid());
		set(_data);
	}

	PointLightComponent &PointLightComponent::set(PointLightData const &data)
	{
		_data = data;
		//AGE::GetPrepareThread()->setPointLight(_data, _key);
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
		set(_data);
	}

#ifdef EDITOR_ENABLED
	void PointLightComponent::editorCreate()
	{}

	void PointLightComponent::editorDelete()
	{}

	bool PointLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(_data.color)))
		{
			set(_data);
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(_data.range), 0.0f, 1.0f))
		{
			set(_data);
			modified = true;
		}
		return modified;
	}
#endif
}