#pragma once
#include <Components/Component.hh>

namespace Component
{
	struct EntityPlacable : public Component::ComponentBase<EntityPlacable>
	{
		EntityPlacable()
		{}

		virtual ~EntityPlacable(void)
		{}

		void init(const std::string &_name)
		{
			name = _name;
		}

		virtual void reset()
		{
		}

		EntityPlacable(EntityPlacable const &o)
		{
			name = o.name;
		}

		EntityPlacable &operator=(EntityPlacable const &o)
		{
			name = o.name;
			return *this;
		}

		std::string name;

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
			auto TODO = 0;
			ar(CEREAL_NVP(TODO));
		}

		// !Serialization
		////
		//////
	private:
	};
}