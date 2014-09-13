#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Core/PrepareElement.hh>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace Component
{

	class PointLight : public ComponentBase<PointLight>, public AGE::PrepareElement
	{
	public:
		PointLight();
		virtual ~PointLight();
		PointLight(PointLight const &o);
		PointLight &operator=(PointLight const &o);
		
		virtual void reset(AScene *);
		void	init(AScene *);

		virtual PrepareElement &initOctree(AScene *scene, ENTITY_ID entityId);
		virtual PrepareElement &resetOctree(AScene *scene, ENTITY_ID entityId);
		PointLight &setPosition(glm::vec4 const &position);
		PointLight &set(float power, float range, glm::vec3 const &color, glm::vec4 const &position);

		template <typename Archive>void serialize(Archive &ar);

	private:
		float _power;
		float _range;
		glm::vec3 _color;
		glm::vec4 _position;
	};

	template <typename Archive>
	void PointLight::serialize(Archive &ar)
	{
	}

}
