#include <Components/SpotLight.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#include "Render/Properties/AutoProperty.hpp"
#include "Render/ProgramResources/Types/Uniform/Vec4.hh"
#include "Render/ProgramResources/Types/Uniform/Vec3.hh"
#include "Render/ProgramResources/Types/Uniform/Vec1.hh"
#include "Render/ProgramResources/Types/Uniform/Mat4.hh"

#include "Graphic/DRBLightElementManager.hpp"
#include "Graphic/DRBData.hpp"
#include "Graphic/DRBSpotLightData.hpp"

#ifdef EDITOR_ENABLED
#	include <imgui\imgui.h>
#	include <glm/gtc/type_ptr.hpp>
#endif

#include <glm/gtc/matrix_transform.hpp>

namespace AGE
{
	SpotLightData::SpotLightData(glm::vec3 const &color, glm::vec3 const &range, float exponent, float cutOff, std::shared_ptr<ITexture> const &map)
		: color(color),
		range(range),
		exponent(exponent),
		cutOff(cutOff),
		map(map)
	{

	}

	SpotLightComponent::SpotLightComponent(SpotLightComponent const &o)
	{
		color = o.color;
		range = o.range;
		exponent = o.exponent;
		cutOff = o.cutOff;
		postUnserialization();
	}

	void SpotLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const SpotLightComponent*>(model);
		color = o->color;
		range = o->range;
		exponent = o->exponent;
		cutOff = o->cutOff;
		postUnserialization();
	}

	void SpotLightComponent::reset()
	{
		_propShadowMatrix = nullptr;
		_propSpotCutOff = nullptr;
		_propPosition = nullptr;
		_propExponentLight = nullptr;
		_propDirection = nullptr;
		_propColorLight = nullptr;
		_propAttenuation = nullptr;

		color = glm::vec4(1.0f);
		range = glm::vec3(1.0f, 0.1f, 0.01f);
		exponent = 5.0f;
		cutOff = 0.5f;

		if (_graphicHandle.invalid() == false)
		{
			auto manager = entity->getScene()->getInstance<DRBLightElementManager>();
			manager->removeSpotLight(_graphicHandle);
			entity->getLink().popAnObject(_graphicHandle);
		}
	}

	void SpotLightComponent::init()
	{
		AGE_ASSERT(_graphicHandle.invalid());

		auto manager = entity->getScene()->getInstance<DRBLightElementManager>();
		_graphicHandle = manager->addSpotLight();

		_propShadowMatrix = std::make_shared<AutoProperty<glm::mat4, Mat4>>("light_matrix");
		_propPosition = std::make_shared<AutoProperty<glm::vec3, Vec3>>("position_light");
		_propAttenuation = std::make_shared<AutoProperty<glm::vec3, Vec3>>("attenuation_light");
		_propAttenuation->autoSet(range);
		_propDirection = std::make_shared<AutoProperty<glm::vec3, Vec3>>("direction_light");
		_propSpotCutOff = std::make_shared<AutoProperty<float, Vec1>>("spot_cut_off");
		_propSpotCutOff->autoSet(cutOff);
		_propExponentLight = std::make_shared<AutoProperty<float, Vec1>>("exponent_light");
		_propExponentLight->autoSet(exponent);
		_propColorLight = std::make_shared<AutoProperty<glm::vec3, Vec3>>("color_light");
		_propColorLight->autoSet(color);

		auto spotLightTexture = entity->getScene()->getInstance<AssetsManager>()->getSpotLightTexture();

		auto &properties = _graphicHandle.getPtr()->getDatas()->globalProperties;
		
		properties.add_property(_propPosition);
		properties.add_property(_propAttenuation);
		auto &castedPropDir = std::static_pointer_cast<AutoProperty<glm::vec3, Vec3>>(_propDirection);
		auto &castedPropPos = std::static_pointer_cast<AutoProperty<glm::vec3, Vec3>>(_propPosition);
		auto &castedPropShadowMatrix = std::static_pointer_cast<AutoProperty<glm::mat4, Mat4>>(_propShadowMatrix);

		std::static_pointer_cast<DRBSpotLightData>(_graphicHandle.getPtr()->getDatas())->registerDirectionProperty(castedPropDir);
		std::static_pointer_cast<DRBSpotLightData>(_graphicHandle.getPtr()->getDatas())->registerPositionProperty(castedPropPos);
		std::static_pointer_cast<DRBSpotLightData>(_graphicHandle.getPtr()->getDatas())->registerShadowMatrixProperty(castedPropShadowMatrix);
		properties.add_property(_propSpotCutOff);
		properties.add_property(_propExponentLight);
		properties.add_property(_propColorLight);

		entity->getLink().pushAnObject(_graphicHandle);
	}

	void SpotLightComponent::postUnserialization()
	{
		init();
	}

#ifdef EDITOR_ENABLED
	void SpotLightComponent::editorCreate()
	{}

	void SpotLightComponent::editorDelete()
	{
	}

	bool SpotLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		{
			_propColorLight->autoSet(glm::vec3(color.x, color.y, color.z));
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(range), 0.0f, 1.0f))
		{
			_propAttenuation->autoSet(range);
			modified = true;
		}
		if (ImGui::InputFloat("Exponent", &exponent))
		{
			_propExponentLight->autoSet(exponent);
			modified = true;
		}
		if (ImGui::InputFloat("cut off", &cutOff))
		{
			_propSpotCutOff->autoSet(cutOff);
			modified = true;
		}
		return modified;
	}
#endif

	glm::mat4 SpotLightComponent::updateShadowMatrix()
	{
		float spotFov = glm::max(0.001f, (1.0f - cutOff) * glm::radians(180.0f));
		// We add 30 degres to have the spot entierly in the frustum (otherwise it is circumscribed)
		float adaptedFov = glm::min(spotFov + glm::radians(30.0f), glm::radians(179.9f));
		glm::mat4 invTransform = glm::inverse(entity->getLink().getGlobalTransform());
		glm::mat4 spotViewProj = glm::perspective(adaptedFov, 1.0f, 0.1f, 1000.0f) * invTransform;
		_propShadowMatrix->autoSet(spotViewProj);
		return (spotViewProj);
	}

}