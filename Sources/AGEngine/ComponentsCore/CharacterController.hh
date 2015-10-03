#pragma once

#include <Components/Component.hh>

#include <Physics/CharacterControllerInterface.hh>

namespace AGE
{
	struct CharacterController final : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_CharacterController");

		CharacterController(void) = default;
		CharacterController(CharacterController const &oth) = delete;

		CharacterController &operator=(CharacterController const &oth) = delete;

		~CharacterController() = default;

		void init(void);
#if defined(EDITOR_ENABLED)
		virtual bool editorUpdate(void) override final;
#endif
		// never copied
		virtual void _copyFrom(const ComponentBase *model);

		template <typename Archive>
		void save(Archive &ar, const std::uint32_t version) const
		{
			ar(controller->getRadius(), controller->getHeight());
		}

		template <typename Archive>
		void load(Archive &ar, const std::uint32_t version)
		{
			float radius, height;
			ar(radius, height);
			if (controller == nullptr)
			{
				init();
			}
			controller->setHeight(height);
			controller->setRadius(radius);
		}

		Physics::CharacterControllerInterface *controller = nullptr;
		uint32_t collisionFlags = 0;
		glm::vec3 velocity;
		glm::vec2 camAngles;
	};
}

