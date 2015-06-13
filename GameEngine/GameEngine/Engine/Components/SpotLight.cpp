#include <Components/SpotLight.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#ifdef EDITOR_ENABLED
#	include <imgui\imgui.h>
#	include <glm/gtc/type_ptr.hpp>
#endif

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
		postUnserialization();
	}

	void SpotLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const SpotLightComponent*>(model);
		postUnserialization();
	}

	void SpotLightComponent::reset()
	{
	}

	void SpotLightComponent::init()
	{
		color = glm::vec3(1.0f);
		range = glm::vec3(1.0f, 0.1f, 0.01f);
		exponent = 5.0f;
		cutOff = 0.5f;

		//_propShadowMap = std::make_shared<Sampler2D>("shadow_map");

		// TODO DORIAN -> fix spotlight texture
		//auto shadowMapTexture = entity->getScene()->getInstance<AssetsManager>()->getSpotLightTexture();
		//_propShadowMap->set(shadowMapTexture);

		//_propShadowMatrix = std::make_shared<Mat4>("light_matrix");
		//_propPosition = std::make_shared<Vec3>("position_light");
		//_propAttenuation = std::make_shared<Vec3>("attenuation_light");
		//_propAttenuation->set(range);
		//_propDirection = std::make_shared<Vec3>("direction_light");
		//_propSpotCutOff = std::make_shared<Vec1>("spot_cuf_off");
		//_propSpotCutOff->set(cutOff);
		//_propExponentLight = std::make_shared<Vec1>("exponent_light");
		//_propExponentLight->set(exponent);
		//_propColorLight = std::make_shared<Vec3>("color_light");
		//_propColorLight->set(glm::vec4(color.x, color.y, color.z, 1.0f));
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
}