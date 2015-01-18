#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Core/PreparableObject.hh>
#include <Utils/GlmSerialization.hpp>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace AGE
{
	namespace Component
	{
		struct PointLight : public ComponentBase < PointLight >
		{
		public:
			PointLight();
			virtual ~PointLight();
			PointLight(PointLight const &o);
			PointLight &operator=(PointLight const &o);

			virtual void reset(AScene *);
			void init(AScene *);

			PointLight &set(glm::vec3 const &color = glm::vec3(1.0f), glm::vec3 const &range = glm::vec3(1.0f, 0.1f, 0.01f));

			template <typename Archive>void serialize(Archive &ar);
			inline const glm::vec3 &getColor() const { return _color; }
			inline const glm::vec3 &getRange() const { return _range; }
			inline float *getColorPtr() { return glm::value_ptr(_color); }
			inline float *getRangePtr() { return glm::value_ptr(_range); }
			virtual void postUnserialization(AScene *scene);
		private:
			AGE::PrepareKey _key;
			AScene *_scene;
			glm::vec3 _color;
			glm::vec3 _range;

			static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
		};

		template <typename Archive>
		void PointLight::serialize(Archive &ar)
		{
			ar(cereal::make_nvp("color", _color), cereal::make_nvp("range", _range));
		}

	}
}