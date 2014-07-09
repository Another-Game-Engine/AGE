#pragma once

#include "Plane.hpp"

namespace AGE
{
	class Frustum
	{
	private:
		enum PlaneName
		{
			NEAR = 0,
			FAR,
			LEFT,
			RIGHT,
			BOTTOM,
			TOP,
			END
		};

		glm::mat4 _matrix;
		Plane _planes[END];
	public:

		void buildPlanes()
		{
			Plane *p = nullptr;

			p = &(_planes[NEAR]);
			p->set(_matrix[0][3] + _matrix[0][2]
				, _matrix[1][3] + _matrix[1][2]
				, _matrix[2][3] + _matrix[2][2]
				, _matrix[3][3] + _matrix[3][2]);
			p->normalize();

			p = &(_planes[FAR]);
			p->set(_matrix[0][3] - _matrix[0][2]
				, _matrix[1][3] - _matrix[1][2]
				, _matrix[2][3] - _matrix[2][2]
				, _matrix[3][3] - _matrix[3][2]);
			p->normalize();

			p = &_planes[LEFT];
			p->set(_matrix[0][3] + _matrix[0][0]
				, _matrix[1][3] + _matrix[1][0]
				, _matrix[2][3] + _matrix[2][0]
				, _matrix[3][3] + _matrix[3][0]);
			p->normalize();

			p = &_planes[RIGHT];
			p->set(_matrix[0][3] - _matrix[0][0]
				, _matrix[1][3] - _matrix[1][0]
				, _matrix[2][3] - _matrix[2][0]
				, _matrix[3][3] - _matrix[3][0]);
			p->normalize();

			p = &_planes[BOTTOM];
			p->set(_matrix[0][3] + _matrix[0][1]
				, _matrix[1][3] + _matrix[1][1]
				, _matrix[2][3] + _matrix[2][1]
				, _matrix[3][3] + _matrix[3][1]);
			p->normalize();

			p = &_planes[TOP];
			p->set(_matrix[0][3] - _matrix[0][1]
				, _matrix[1][3] - _matrix[1][1]
				, _matrix[2][3] - _matrix[2][1]
				, _matrix[3][3] - _matrix[3][1]);
			p->normalize();
		}

		void setMatrix(const glm::mat4 &matrix, bool rebuildPlanes = false)
		{
			_matrix = matrix;
			if (rebuildPlanes)
				buildPlanes();
		}

		bool pointIn(const glm::vec3 &point) const
		{
			for (int i = 0; i < END; ++i)
			{
				if (_planes[i].dot(point) <= 0.0f)
					return false;
			}
			return true;
		}
	};
}