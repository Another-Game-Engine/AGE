#pragma once

#include <Components/Component.hh>

namespace AGE
{
	struct Lifetime : public ComponentBase
	{
		Lifetime();
		virtual ~Lifetime(void);
		Lifetime &operator=(const Lifetime &o) = delete;
		virtual void _copyFrom(const ComponentBase *model);

		void init(float t = 1.0f);

		virtual void reset();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(CEREAL_NVP(_t));
		}

		// !Serialization
		////
		//////

		float _t;
	};
}

CEREAL_CLASS_VERSION(AGE::Lifetime, 0)