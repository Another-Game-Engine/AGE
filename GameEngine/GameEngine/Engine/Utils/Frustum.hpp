#pragma once

#include "Plane.hpp"

namespace AGE
{
	class Frustum
	{
	public:
		enum PlaneName
		{
			NEAR = 0,
			FAR,
			LEFT,
			RIGHT,
			BOTTOM,
			TOP,
			__END
		};

		void setMatrix(const glm::mat4 &matrix, bool rebuildPlanes = false)
		{
			_matrix = matrix;
			if (rebuildPlanes)
				buildPlanes();
		}

		void buildPlanes()
		{
			Plane *p = nullptr;
			
			p = &_planes[NEAR];
			p->set(_matrix[0][3] + _matrix[0][2]
				, _matrix[1][3] + _matrix[1][2]
				, _matrix[3][4] + _matrix[3][3]
				, _matrix[4][4] + _matrix[4][3]);
			p->normalize();

			p = &_planes[FAR];
			p->set(_matrix[0][3] - _matrix[0][2]
				, _matrix[1][3] - _matrix[1][2]
				, _matrix[3][4] - _matrix[3][3]
				, _matrix[4][4] - _matrix[4][3]);
			p->normalize();

			p = &_planes[LEFT];
			p->set(_matrix[0][3] + _matrix[0][0]
				,_matrix[1][3] + _matrix[1][0]
				,_matrix[3][4] + _matrix[3][0]
				,_matrix[4][4] + _matrix[4][0]);
			p->normalize();

			p = &_planes[RIGHT];
			p->set(_matrix[0][3] - _matrix[0][0]
				,_matrix[1][3] - _matrix[1][0]
				,_matrix[3][4] - _matrix[3][0]
				,_matrix[4][4] - _matrix[4][0]);
			p->normalize();

			p = &_planes[BOTTOM];
			p->set(_matrix[0][3] + _matrix[0][1]
				,_matrix[1][3] + _matrix[1][1]
				,_matrix[3][4] + _matrix[3][1]
				,_matrix[4][4] + _matrix[4][1]);
			p->normalize();

			p = &_planes[FAR];
			p->set(_matrix[0][3] - _matrix[0][1]
				,_matrix[1][3] - _matrix[1][1]
				,_matrix[3][4] - _matrix[3][1]
				,_matrix[4][4]- _matrix[4][1]);
			p->normalize();
		}
	private:
		glm::mat4 _matrix;
		Plane _planes[__END];
	};
}