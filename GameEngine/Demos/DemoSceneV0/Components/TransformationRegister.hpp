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
		TransformationRegister(TransformationRegister const &);
		TransformationRegister &operator=(TransformationRegister const &);
	};
}