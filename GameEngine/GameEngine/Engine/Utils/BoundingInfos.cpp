#include <Utils/BoundingInfos.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AGE
{
	BoundingInfos::BoundingInfos()
		: _center(0)
		, _min(std::numeric_limits<float>::max())
		, _max(std::numeric_limits<float>::min())
		, _size(0)
	{}

	void BoundingInfos::recompute()
	{
		_size = _max - _min;
		_center = glm::vec3((_min.x + _max.x) / 2, (_min.y + _max.y) / 2, (_min.z + _max.z) / 2);
		_transform = glm::translate(glm::mat4(1), _center) * glm::scale(glm::mat4(1), _size);
	}

	void BoundingInfos::initFromList(const std::vector<BoundingInfos> &col)
	{
		_center = glm::vec3(0);
		_min = glm::vec3(std::numeric_limits<float>::max());
		_max = glm::vec3(std::numeric_limits<float>::min());
		_size = glm::vec3(0);

		for (auto &e : col)
		{
			addPosition(e._min);
			addPosition(e._max);
		}
	}

	void BoundingInfos::addPosition(const glm::vec3 &v)
	{
		bool refresh = false;

		// min
		if (v.x < _min.x)
		{
			_min.x = v.x;
			refresh = true;
		}
		if (v.y < _min.y)
		{
			_min.y = v.y;
			refresh = true;
		}
		if (v.z < _min.z)
		{
			_min.z = v.z;
			refresh = true;
		}

		// max
		if (v.x > _max.x)
		{
			_max.x = v.x;
			refresh = true;
		}
		if (v.y > _max.y)
		{
			_max.y = v.y;
			refresh = true;
		}
		if (v.z > _max.z)
		{
			_max.z = v.z;
			refresh = true;
		}
		if (refresh)
			recompute();
	}
}