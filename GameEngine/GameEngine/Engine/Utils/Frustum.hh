#pragma once

#include "Plane.hpp"

namespace AGE
{
	class Frustum
	{
	private:
		enum PlaneName
		{
			_NEAR,
			_FAR,
			_LEFT,
			_RIGHT,
			_BOTTOM,
			_TOP,
			_END
		};

		glm::mat4 _matrix;
		Plane _planes[_END];
	public:

		void buildPlanes();
		void setMatrix(const glm::mat4 &matrix, bool rebuildPlanes = false);
		bool pointIn(const glm::vec3 &point) const;
	};
}