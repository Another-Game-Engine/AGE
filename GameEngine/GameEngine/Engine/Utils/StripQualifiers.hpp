#ifndef  __STRIP_QUALIFIERS_HPP__
# define __STRIP_QUALIFIERS_HPP__

#include <memory>

template <typename T>
struct StripQual
{
	typedef T type;
};

template <typename T>
struct StripQual<const T>
{
	typedef T type;
};

template <typename T>
struct StripQual<T&>
{
	typedef T type;
};

template <typename T>
struct StripQual<const T&>
{
	typedef T type;
};

template <typename T>
struct StripQual<T&&>
{
	typedef T type;
};

template <typename T>
struct StripQual<T*>
{
	typedef T type;
};

template <typename T>
struct StripQual<const T*>
{
	typedef T type;
};


#endif   //__STRIP_QUALIFIERS_HPP__