#include "AnimationChannel.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_interpolation.hpp>


using namespace AGE;

void AnimationChannel::findKeyIndex(float t, glm::uvec3 &keys, glm::uvec3 &nextKeys)
{
	if (scale.size() <= 1)
	{
		keys.x = 0;
		nextKeys.x = 0;
	}
	else
	{
		for (unsigned int i = 0; i < scale.size() - 1; i++)
		{
			if (t < scale[i + 1].time) {
				keys.x = i;
				nextKeys.x = i + 1;
				break;
			}
		}
	}

	if (rotation.size() <= 1)
	{
		keys.y = 0;
		nextKeys.y = 0;
	}
	else
	{
		for (unsigned int i = 0; i < rotation.size() - 1; i++)
		{
			if (t < rotation[i + 1].time) {
				keys.y = i;
				nextKeys.y = i + 1;
				break;
			}
		}
	}

	if (translation.size() <= 1)
	{
		keys.z = 0;
		nextKeys.z = 0;
	}
	else
	{
		for (unsigned int i = 0; i < translation.size() - 1; i++)
		{
			if (t < translation[i + 1].time) {
				keys.z = i;
				nextKeys.z = i + 1;
				break;
			}
		}
	}
}

void AnimationChannel::getInterpolatedTransform(float t, glm::mat4 &res)
{
	glm::uvec3 key = glm::uvec3(static_cast<unsigned int>(t));
	glm::uvec3 nextKey = glm::uvec3(key.x + 1);
	
	if (nextKey.z >= translation.size() || nextKey.x >= scale.size() || nextKey.y >= rotation.size())
		findKeyIndex(t, key, nextKey);
	
	res = glm::translate(glm::mat4(1), glm::mix(translation[key.z].value, translation[nextKey.z].value, (t - translation[key.z].time) / translation[key.z].deltaTime));
	res *= glm::scale(glm::mat4(1), glm::mix(scale[key.x].value, scale[nextKey.x].value, (t - scale[key.x].time) / scale[key.x].deltaTime));
	res *= glm::toMat4(glm::slerp(rotation[key.y].value, rotation[nextKey.y].value, (t - rotation[key.y].time) / rotation[key.y].deltaTime));
}
