#pragma once

#include <Components/Component.hh>
#include <Render/Key.hh>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <Render/Textures/ITexture.hh>
#include <memory>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace AGE
{
	class IProperty;

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

		inline BFCCullableHandle getCullableHandle() const { return _graphicHandle; }

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

	protected:
		void _updateProperties();

	private:
		glm::vec3 color;
		glm::vec3 range;
		std::shared_ptr<ITexture> map;

		BFCCullableHandle          _graphicHandle;

		std::shared_ptr<IProperty> _mapProp = nullptr;
		std::shared_ptr<IProperty> _rangeProperty = nullptr;
		std::shared_ptr<IProperty> _colorProperty = nullptr;
		std::shared_ptr<IProperty> _ambiantColorProperty = nullptr;

		static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
	};

	template <typename Archive>
	void PointLightComponent::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(cereal::make_nvp("color", color), cereal::make_nvp("range", range));
	}
}

CEREAL_CLASS_VERSION(AGE::PointLightComponent, 1)
