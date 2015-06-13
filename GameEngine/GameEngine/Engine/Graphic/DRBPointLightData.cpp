#include "DRBPointLightData.hpp"

namespace AGE
{
	void DRBPointLightData::setRange(const glm::vec3 &range)
	{
		RWLockGuard(_lock, true);
		_range = range;
	}

	const glm::vec3 DRBPointLightData::getRange() const
	{
		RWLockGuard(_lock, false);
		return _range;
	}
}