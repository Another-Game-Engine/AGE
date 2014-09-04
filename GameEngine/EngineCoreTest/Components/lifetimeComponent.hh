#pragma once

#include <Components/Component.hh>

namespace Component
{
	struct Lifetime : public Component::ComponentBase<Lifetime>
	{
		Lifetime();
		virtual ~Lifetime();
		Lifetime(Lifetime &&o);
		Lifetime &operator=(Lifetime &&o);

		void init(AScene *, float t);
		virtual void reset(AScene *);

		template <typename Archive> void serialize(Archive &ar);

		float _t;
	private:
		Lifetime &operator=(Lifetime const &o) = delete;
		Lifetime(Lifetime const &o) = delete;
	};

	template <typename Archive>
	void Lifetime::serialize(Archive &ar)
	{
		ar(CEREAL_NVP(_t));
	}
}