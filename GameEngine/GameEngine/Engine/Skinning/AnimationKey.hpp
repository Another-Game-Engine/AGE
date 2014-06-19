#pragma once

namespace AGE
{
	template <typename T>
	struct AnimationKey
	{
		T value;
		float time;
		float deltaTime;

		AnimationKey(const T &_value, float _time)
			: value(_value)
			, time(_time)
		{}
		AnimationKey(T &&_value, float _time)
			: value(std::move(_value))
			, time(_time)
		{}
	};
}