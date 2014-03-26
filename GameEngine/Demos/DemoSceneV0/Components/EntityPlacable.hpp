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

		std::string name;

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new TransformationRegister();
			res->setEntity(e);
			ar(*res);
			return res;
		}

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
		EntityPlacable(EntityPlacable const &);
		EntityPlacable &operator=(EntityPlacable const &);
	};
}