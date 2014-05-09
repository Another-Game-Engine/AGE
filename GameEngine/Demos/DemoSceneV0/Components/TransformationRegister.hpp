#pragma once
#include <Components/Component.hh>

namespace Component
{
	struct TransformationRegister : public Component::ComponentBase<TransformationRegister>
	{
		TransformationRegister()
		{}

		virtual ~TransformationRegister(void)
		{}

		void init(const std::string &_name)
		{
			name = _name;
		}

		virtual void reset()
		{
		}

		TransformationRegister(TransformationRegister const &o)
		{
			name = o.name;
		}

		TransformationRegister &operator=(TransformationRegister const &o)
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