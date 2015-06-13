#include <Components/Light.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <Threads/ThreadManager.hpp>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>
#include "Render\Properties\Materials\Color.hh"
#include "Render\Properties\Materials\MapColor.hh"
#include "Graphic\DRBLightElementManager.hpp"

#include "Graphic/DRBData.hpp"
#include "Graphic/DRBPointLightData.hpp"

#ifdef EDITOR_ENABLED
#include <imgui\imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	
	PointLightComponent::PointLightComponent(PointLightComponent const &o)
		: color(o.color),
		range(o.range),
		map(o.map)
	{
		postUnserialization();
	}

	void PointLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const PointLightComponent*>(model);
		range = o->range;
		color = o->color;
		postUnserialization();
	}

	void PointLightComponent::reset()
	{
		color = glm::vec3(1);
		range = glm::vec3(1.0f, 0.01f, 0.001f);
		map = nullptr;
		_colorProp = nullptr;
		_mapProp = nullptr;

		if (_graphicHandle.invalid() == false)
		{
			auto manager = entity->getScene()->getInstance<DRBLightElementManager>();
			manager->removePointLight(_graphicHandle);
		}
	}

	void PointLightComponent::init()
	{
		_colorProp = std::make_shared<Color>("color");
		_mapProp = std::make_shared<MapColor>("sprite_light");

		auto manager = entity->getScene()->getInstance<DRBLightElementManager>();
		_graphicHandle = manager->addPointLight(_colorProp, _mapProp);

		_graphicHandle.getPtr()->getDatas()->globalProperties.add_property(_colorProp);
		_graphicHandle.getPtr()->getDatas()->globalProperties.add_property(_mapProp);
		std::static_pointer_cast<DRBPointLightData>(_graphicHandle.getPtr()->getDatas())->setRange(range);

		map = entity->getScene()->getInstance<AssetsManager>()->getPointLightTexture();
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
		//set(_data);
	}

#ifdef EDITOR_ENABLED
	void PointLightComponent::editorCreate()
	{}

	void PointLightComponent::editorDelete()
	{}

	bool PointLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		{
			//set(_data);
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(range), 0.0f, 1.0f))
		{
			//set(_data);
			modified = true;
		}
		return modified;
	}
#endif
}