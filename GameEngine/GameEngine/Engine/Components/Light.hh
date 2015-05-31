#pragma once

#include <Components/Component.hh>
#include <Render/Key.hh>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <Render/Textures/ITexture.hh>
#include <memory>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace AGE
{
	struct PointLightData
	{
		PointLightData(glm::vec3 const &color = glm::vec3(1.0f), 
						glm::vec3 const &range = glm::vec3(1.0f, 0.1f, 0.01f),
						std::shared_ptr<ITexture> const &map = nullptr);
		glm::vec3 color;
		glm::vec3 range;
		std::shared_ptr<ITexture> map;
	};

	struct PointLightComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_PointLightComponent");
	public:
		PointLightComponent() = default;
		virtual ~PointLightComponent() = default;
		PointLightComponent(PointLightComponent const &o);

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();

		PointLightComponent &set(PointLightData const &data);

		template <typename Archive>void serialize(Archive &ar, const std::uint32_t version);
		inline const PointLightData &get() const;
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

	private:
		AGE::PrepareKey _key;
		PointLightData _data;

#ifdef EDITOR_ENABLED
#endif

		static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
	};

	template <typename Archive>
	void PointLightComponent::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(cereal::make_nvp("color", _data.color), cereal::make_nvp("range", _data.range));
	}
}

CEREAL_CLASS_VERSION(AGE::PointLightComponent, 0)
