#pragma once

#include "Frustum.hh"

namespace AGE
{

	void	Frustum::buildPlanes()
	{
		Plane *p = nullptr;

		p = &(_planes[_NEAR]);
		p->set(_matrix[0][3] + _matrix[0][2],
			_matrix[1][3] + _matrix[1][2],
			_matrix[2][3] + _matrix[2][2],
			_matrix[3][3] + _matrix[3][2]);
		p->normalize();

		p = &(_planes[_FAR]);
		p->set(_matrix[0][3] - _matrix[0][2],
			_matrix[1][3] - _matrix[1][2],
			_matrix[2][3] - _matrix[2][2],
			_matrix[3][3] - _matrix[3][2]);
		p->normalize();

		p = &_planes[_LEFT];
		p->set(_matrix[0][3] + _matrix[0][0],
			_matrix[1][3] + _matrix[1][0],
			_matrix[2][3] + _matrix[2][0],
			_matrix[3][3] + _matrix[3][0]);
		p->normalize();

		p = &_planes[_RIGHT];
		p->set(_matrix[0][3] - _matrix[0][0],
			_matrix[1][3] - _matrix[1][0],
			_matrix[2][3] - _matrix[2][0],
			_matrix[3][3] - _matrix[3][0]);
		p->normalize();

		p = &_planes[_BOTTOM];
		p->set(_matrix[0][3] + _matrix[0][1],
			_matrix[1][3] + _matrix[1][1],
			_matrix[2][3] + _matrix[2][1],
			_matrix[3][3] + _matrix[3][1]);
		p->normalize();

		p = &_planes[_TOP];
		p->set(_matrix[0][3] - _matrix[0][1],
			_matrix[1][3] - _matrix[1][1],
			_matrix[2][3] - _matrix[2][1],
			_matrix[3][3] - _matrix[3][1]);
		p->normalize();
	}

	void Frustum::setMatrix(const glm::mat4 &matrix, bool rebuildPlanes)
	{
		_matrix = matrix;
		if (rebuildPlanes)
			buildPlanes();
	}

	bool Frustum::pointIn(const glm::vec3 &point) const
	{
		for (int i = 0; i < _END; ++i)
		{
			auto v = _planes[i].dot(point);
			if (v <= 0.0f)
				return false;
		}
		return true;
	}

}