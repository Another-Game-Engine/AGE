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
	public:
		PointLightComponent();
		virtual ~PointLightComponent();
		PointLightComponent(PointLightComponent const &o);

		virtual void _copyFrom(const ComponentBase *model);

		virtual void reset();
		void init();

		PointLightComponent &set(glm::vec3 const &color = glm::vec3(1.0f), glm::vec3 const &range = glm::vec3(1.0f, 0.1f, 0.01f));

		template <typename Archive>void serialize(Archive &ar, const std::uint32_t version);
		inline const glm::vec3 &getColor() const { return _color; }
		inline const glm::vec3 &getRange() const { return _range; }
		inline float *getColorPtr() { return &_color.x; }
		inline float *getRangePtr() { return &_range.x; }
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual void editorUpdate();
#endif

	private:
		AGE::PrepareKey _key;
		glm::vec3 _color;
		glm::vec3 _range;
		std::shared_ptr<ITexture> _map;

#ifdef EDITOR_ENABLED
#endif

		static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
	};

	template <typename Archive>
	void PointLightComponent::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(cereal::make_nvp("color", _color), cereal::make_nvp("range", _range));
	}
}

CEREAL_CLASS_VERSION(AGE::PointLightComponent, 0)
