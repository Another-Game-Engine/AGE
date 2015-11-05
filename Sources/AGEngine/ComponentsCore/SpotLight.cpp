#include <ComponentsCore/SpotLight.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#include "Render/ProgramResources/Types/Uniform/Vec4.hh"
#include "Render/ProgramResources/Types/Uniform/Vec3.hh"
#include "Render/ProgramResources/Types/Uniform/Vec1.hh"
#include "Render/ProgramResources/Types/Uniform/Mat4.hh"

#include "Graphic/DRBLightElementManager.hpp"
#include "Graphic/DRBData.hpp"

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

		entity->getLink().pushAnObject(_graphicHandle);
	}

	void SpotLightComponent::postUnserialization()
	{
		init();
	}

#ifdef EDITOR_ENABLED
	bool SpotLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		{
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(range), 0.0f, 1.0f))
		{
			modified = true;
		}
		if (ImGui::InputFloat("Exponent", &exponent))
		{
			modified = true;
		}
		if (ImGui::InputFloat("cut off", &cutOff))
		{
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
		return (spotViewProj);
	}

	glm::vec3 SpotLightComponent::getDirection() const
	{
		auto direction = glm::transpose(glm::inverse(glm::mat3(entity->getLink().getGlobalTransform()))) * glm::vec3(0.0f, 0.0f, -1.0f);
		return direction;
	}

	glm::vec3 SpotLightComponent::getColor() const
	{
		return glm::vec3(color);
	}

	glm::vec3 SpotLightComponent::getPosition() const
	{
		auto position = glm::vec3(entity->getLink().getGlobalTransform()[3]);
		return position;
	}

	glm::vec3 SpotLightComponent::getAttenuation() const
	{
		return range;
	}


}