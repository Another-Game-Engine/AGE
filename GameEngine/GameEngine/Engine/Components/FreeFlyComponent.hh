#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/SerializationArchives.hpp>

#include <Engine/Core/Inputs/AgeInputs.hh>

namespace AGE
{
	// Very basic FreeFly component for the moment.
	// It might contain more information in the future
	struct FreeFlyComponent : public ComponentBase
	{
		// For the keyboard
		AgeKeys accelerate;
		AgeKeys translateForward;
		AgeKeys translateBackward;
		AgeKeys translateLeft;
		AgeKeys translateRight;
		AgeKeys rotateUp;
		AgeKeys rotateDown;
		AgeKeys rotateLeft;
		AgeKeys rotateRight;

		// For controller and keyboard
		float camTranslationSpeed;
		float maxAcceleration;
		float camRotationSpeed;

		// For the mouse
		AgeMouseButtons mouseButton;
		float camMouseRotationSpeed;

		bool handleKeyboard;
		bool handleMouse;
		bool handleController;

		void init();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(accelerate, translateForward,
				translateBackward, translateLeft,
				translateRight, rotateUp,
				rotateDown, rotateLeft,
				rotateRight, camTranslationSpeed,
				maxAcceleration, camRotationSpeed,
				mouseButton, camMouseRotationSpeed,
				handleKeyboard, handleMouse,
				handleController);
		}

		// !Serialization
		////
		//////
	};
}

CEREAL_CLASS_VERSION(AGE::FreeFlyComponent, 0)