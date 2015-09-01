#pragma once

#include <Components/Component.hh>

namespace AGE
{
	struct CharacterController : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_CharacterController");

		CharacterController(void) = default;
		CharacterController(CharacterController const &oth) = delete;

		CharacterController &operator=(CharacterController const &oth) = delete;

		~CharacterController() = default;

		void init(void);
		virtual void editorCreate(void) override final;
		virtual void editorDelete(void) override final;
		virtual bool editorUpdate(void) override final;

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
		}

	};
}

