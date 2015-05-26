#pragma once

#include <Components/Component.hh>
#include <glm/fwd.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <Render/Textures/ITexture.hh>

namespace AGE
{

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

		void set(SpotLightData const &data);

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(cereal::make_nvp("color", _data.color), cereal::make_nvp("range", _data.range), cereal::make_nvp("exponent", _data.exponent), cereal::make_nvp("cutOff", _data.cutOff));
		}
		inline const SpotLightData &get() const { return _data; }
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

	private:
		AGE::PrepareKey _key;
		SpotLightData _data;

	};
}