#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Core/PreparableObject.hh>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace Component
{

	struct PointLight : public ComponentBase<PointLight>
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

	private:
		AGE::PrepareKey _key;
		AScene *_scene;

		static float computePointLightRange(float minValue, glm::vec3 const &attenuation);
	};

	template <typename Archive>
	void PointLight::serialize(Archive &ar)
	{
	}

}
