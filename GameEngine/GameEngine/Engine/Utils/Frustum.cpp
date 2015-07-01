#pragma once

#include "Frustum.hh"

namespace AGE
{

	void	Frustum::buildPlanes()
	{
		_planes[PLANE_NEAR].setCoefficients(
			_viewProj[0][2] + _viewProj[0][3],
			_viewProj[1][2] + _viewProj[1][3],
			_viewProj[2][2] + _viewProj[2][3],
			_viewProj[3][2] + _viewProj[3][3]);
		_planes[PLANE_FAR].setCoefficients(
			-_viewProj[0][2] + _viewProj[0][3],
			-_viewProj[1][2] + _viewProj[1][3],
			-_viewProj[2][2] + _viewProj[2][3],
			-_viewProj[3][2] + _viewProj[3][3]);
		_planes[PLANE_BOTTOM].setCoefficients(
			_viewProj[0][1] + _viewProj[0][3],
			_viewProj[1][1] + _viewProj[1][3],
			_viewProj[2][1] + _viewProj[2][3],
			_viewProj[3][1] + _viewProj[3][3]);
		_planes[PLANE_TOP].setCoefficients(
			-_viewProj[0][1] + _viewProj[0][3],
			-_viewProj[1][1] + _viewProj[1][3],
			-_viewProj[2][1] + _viewProj[2][3],
			-_viewProj[3][1] + _viewProj[3][3]);
		_planes[PLANE_LEFT].setCoefficients(
			_viewProj[0][0] + _viewProj[0][3],
			_viewProj[1][0] + _viewProj[1][3],
			_viewProj[2][0] + _viewProj[2][3],
			_viewProj[3][0] + _viewProj[3][3]);
		_planes[PLANE_RIGHT].setCoefficients(
			-_viewProj[0][0] + _viewProj[0][3],
			-_viewProj[1][0] + _viewProj[1][3],
			-_viewProj[2][0] + _viewProj[2][3],
			-_viewProj[3][0] + _viewProj[3][3]);
	}

	void Frustum::setMatrix(const glm::mat4 &viewProj)
	{
		if (_viewProj != viewProj)
		{
			_viewProj = viewProj;
			buildPlanes();
		}
	}

	bool Frustum::checkCollision(AABoundingBox const &aabb) const
	{
		for (int i = 0; i < PLANE_END; i++)
		{
			if (_planes[i].getPointDistance(aabb.getPositivePoint(_planes[i].getNormal())) < 0)
				return (false);
		}
		return (true);
	}

	bool Frustum::checkCollision(Sphere const &sphere) const
	{
		assert(!"Not implemented");
		return (false);
	}

	bool Frustum::checkCollision(glm::vec4 const &sphere) const
	{
		for (int i = 0; i < PLANE_END; i++)
		{
			glm::vec3 ret = glm::vec3(sphere.x, sphere.y, sphere.z) - glm::vec3(sphere.w);
			glm::vec3 normal = _planes[i].getNormal();

			if (normal.x >= 0)
				ret.x += sphere.w * 2;
			if (normal.y >= 0)
				ret.y += sphere.w * 2;
			if (normal.z >= 0)
				ret.z += sphere.w * 2;

			if (_planes[i].getPointDistance(ret) < 0)
				return (false);
		}
		return (true);
	}

	bool Frustum::checkCollision(Frustum const &frustum) const
	{
		assert(!"Not implemented");
		return (false);
	}


}