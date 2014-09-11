#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Core/OctreeElement.hh>

namespace gl { class GeometryManager; class Vertices; class Indices; }

namespace Component
{

	struct PointLight : public ComponentBase<PointLight>, public AGE::OctreeElement
	{
	public:
		float power;
		float range;
		glm::vec3 color;
		glm::vec4 position;
	
	public:
		PointLight();
		virtual ~PointLight();
		PointLight(PointLight const &o);
		PointLight &operator=(PointLight const &o);
		
		virtual void reset(AScene *);
		void	init(AScene *);

		virtual OctreeElement &initOctree(AScene *scene, ENTITY_ID entityId);
		virtual OctreeElement &resetOctree(AScene *scene, ENTITY_ID entityId);

		template <typename Archive>void serialize(Archive &ar);
	};

	template <typename Archive>
	void PointLight::serialize(Archive &ar)
	{
	}

}
