#pragma once

#include <Utils/Plane.hh>
#include <Utils/AABoundingBox.hh>
#include <Utils/Sphere.hh>
#include <SpacePartitioning/Cullable/Cullable.hh>

namespace AGE
{

	class Frustum
	{
	public:
		enum { shape_type = CULLABLE_FRUSTUM };

	private:
		enum PlaneName
		{
			PLANE_NEAR,
			PLANE_FAR,
			PLANE_LEFT,
			PLANE_RIGHT,
			PLANE_BOTTOM,
			PLANE_TOP,
			PLANE_END
		};

		glm::mat4 _viewProj;
		Plane _planes[PLANE_END];
	public:

		void buildPlanes();
		void setMatrix(const glm::mat4 &viewProj);

		// check the collision between AABB and frustum
		bool checkCollision(AABoundingBox const &aabb) const;
		bool checkCollision(Sphere const &sphere) const;
		bool checkCollision(Frustum const &frustum) const;
	};
}