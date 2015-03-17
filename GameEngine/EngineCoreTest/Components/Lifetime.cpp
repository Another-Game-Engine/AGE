#include "Lifetime.hpp"

namespace AGE
{
	Lifetime::Lifetime()
	{

	}

	Lifetime::~Lifetime(void)
	{

	}


	void Lifetime::init(float t/* = 1.0f*/)
	{
		_t = t;
	}

	void Lifetime::reset()
	{
		_t = 0.0f;
	}
}