#pragma once

#include <Components/Component.hh>
#include <Render/Key.hh>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <Render/Textures/ITexture.hh>
#include <memory>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace AGE
{
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

		template <typename Archive>void serialize(Archive &ar, const std::uint32_t version);
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

	private:
		glm::vec3 color;
		glm::vec3 range;
		std::shared_ptr<ITexture> map;

#ifdef EDITOR_ENABLED
#endif

		static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
	};

	template <typename Archive>
	void PointLightComponent::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(cereal::make_nvp("color", color), cereal::make_nvp("range", range));
	}
}

CEREAL_CLASS_VERSION(AGE::PointLightComponent, 1)
