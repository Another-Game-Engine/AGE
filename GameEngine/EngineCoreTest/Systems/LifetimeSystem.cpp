#include <Systems/LifetimeSystem.hpp>

namespace Component
{
	Lifetime::Lifetime()
	{

	}

	Lifetime::~Lifetime()
	{

	}

	Lifetime::Lifetime(Lifetime &&o)
		: ComponentBase<Lifetime>(std::move(o))
	{
			_t = std::move(o._t);
	}

	Lifetime &Lifetime::operator=(Lifetime &&o)
	{
		_t = std::move(o._t);
		return *this;
	}

	void Lifetime::init(AScene *, float t)
	{
		_t = t;
	}

	void Lifetime::reset(AScene *)
	{
		_t = 0.0f;
	}
}