#pragma once
namespace BitsetManipulation
{

	template <typename T>
	static set(T &v, std::uint8_t t)
	{
		v |= (0 << t);
	}

	template <typename T>
	static unset(T &v, std::uint8_t t)
	{
		components &= ~((T)(-1) << t);
	}
}