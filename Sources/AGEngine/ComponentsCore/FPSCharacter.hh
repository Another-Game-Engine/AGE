#pragma once

#include <Components/Component.hh>

namespace AGE
{
	struct FPSCharacter final : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_FPSCharacter");

		FPSCharacter(void) = default;
		FPSCharacter(FPSCharacter const &oth) = delete;

		FPSCharacter &operator=(FPSCharacter const &oth) = delete;

		~FPSCharacter() = default;

		void init(void);
		virtual void editorCreate(void) override final;
		virtual void editorDelete(void) override final;
		virtual bool editorUpdate(void) override final;

		// never copied
		virtual void _copyFrom(const ComponentBase *model);

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
		}
	};
}

