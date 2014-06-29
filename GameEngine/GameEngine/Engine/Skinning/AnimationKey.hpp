#pragma once


#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

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
		AnimationKey()
			: time(0)
		{}
	};

	template <class Archive, class T>
	void serialize(Archive &ar, AnimationKey<T> &value)
	{
		ar(cereal::make_nvp("value", value.value));
		ar(cereal::make_nvp("time", value.time));
		ar(cereal::make_nvp("deltaTime", value.deltaTime));
	}

}