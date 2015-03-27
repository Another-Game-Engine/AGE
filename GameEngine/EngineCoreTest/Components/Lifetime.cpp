#include "Lifetime.hpp"

namespace AGE
{
	Lifetime::Lifetime()
	{

	}

	Lifetime::~Lifetime(void)
	{

	}

	void Lifetime::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const Lifetime*>(model);
		_t = o->_t;
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