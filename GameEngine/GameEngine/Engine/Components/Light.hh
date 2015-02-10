#pragma once

#include <Components/Component.hh>
#include <Render/Key.hh>
#include <Core/PreparableObject.hh>
#include <Utils/Serialization/VectorSerialization.hpp>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace AGE
{
	struct PointLightComponent : public ComponentBase
	{
	public:
		PointLightComponent();
		virtual ~PointLightComponent();
		PointLightComponent(PointLightComponent const &o);
		PointLightComponent &operator=(PointLightComponent const &o);

		virtual void reset(AScene *);
		void init(AScene *);

		PointLightComponent &set(glm::vec3 const &color = glm::vec3(1.0f), glm::vec3 const &range = glm::vec3(1.0f, 0.1f, 0.01f));

		template <typename Archive>void serialize(Archive &ar);
		inline const glm::vec3 &getColor() const { return _color; }
		inline const glm::vec3 &getRange() const { return _range; }
		inline float *getColorPtr() { return &_color.x; }
		inline float *getRangePtr() { return &_range.x; }
		virtual void postUnserialization(AScene *scene);

#ifdef EDITOR_ENABLED
		virtual void editorCreate(AScene *scene);
		virtual void editorDelete(AScene *scene);
		virtual void editorUpdate(AScene *scene);
#endif

	private:
		AGE::PrepareKey _key;
		AScene *_scene;
		glm::vec3 _color;
		glm::vec3 _range;

		static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
	};

	template <typename Archive>
	void PointLightComponent::serialize(Archive &ar)
	{
		ar(cereal::make_nvp("color", _color), cereal::make_nvp("range", _range));
	}

}
