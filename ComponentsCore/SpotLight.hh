#pragma once

#include <Components/Component.hh>
#include <glm/fwd.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <Render/Textures/ITexture.hh>

namespace AGE
{
	class IProperty;
	class Sampler2D;
	class Mat4;
	class Vec3;
	class Vec1;
	class BFCCullableHandle;

	struct SpotLightData
	{
		SpotLightData(glm::vec3 const &color = glm::vec3(1.0f), 
					  glm::vec3 const &range = glm::vec3(1.0f, 0.1f, 0.01f), 
					  float exponent = 5.0f, 
					  float cutOff = 0.5f, 
					  std::shared_ptr<ITexture> const &map = nullptr);
		glm::vec3 color;
		glm::vec3 range;
		float exponent;
		float cutOff;
		std::shared_ptr<ITexture> map;
	};

	struct SpotLightComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_SpotLightComponent");
	public:
		SpotLightComponent() = default;
		virtual ~SpotLightComponent() = default;
		SpotLightComponent(SpotLightComponent const &o);

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();

		inline BFCCullableHandle getCullableHandle() const { return _graphicHandle; }

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(cereal::make_nvp("color", color), cereal::make_nvp("range", range), cereal::make_nvp("exponent", exponent), cereal::make_nvp("cutOff", cutOff));
		}
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif
		inline float getCutOff() const { return cutOff; }
		inline float getExponent() const { return exponent; }

		glm::mat4 updateShadowMatrix();

	private:
		glm::vec4 color;
		glm::vec3 range;
		float exponent;
		float cutOff;

		// TODO
		std::shared_ptr<IProperty>      _propShadowMatrix;
		std::shared_ptr<IProperty>      _propPosition;
		std::shared_ptr<IProperty>      _propAttenuation;
		std::shared_ptr<IProperty>      _propDirection;
		std::shared_ptr<IProperty>      _propSpotCutOff;
		std::shared_ptr<IProperty>      _propExponentLight;
		std::shared_ptr<IProperty>      _propColorLight;
		BFCCullableHandle               _graphicHandle;
	};
}

CEREAL_CLASS_VERSION(AGE::SpotLightComponent, 0);
