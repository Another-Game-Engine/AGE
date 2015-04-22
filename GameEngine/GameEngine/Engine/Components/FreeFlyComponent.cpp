#include <Components/FreeFlyComponent.hh>

namespace AGE
{
	void FreeFlyComponent::init()
	{
		accelerate = AGE_LSHIFT;
		translateForward = AGE_w;
		translateBackward = AGE_s;
		translateLeft = AGE_a;
		translateRight = AGE_d;
		rotateUp = AGE_UP;
		rotateDown = AGE_DOWN;
		rotateLeft = AGE_LEFT;
		rotateRight = AGE_RIGHT;
		camTranslationSpeed = 5.0f;
		maxAcceleration = 10.0f;
		camRotationSpeed = 2.0f;
		camMouseRotationSpeed = 0.0005f;
		mouseButton = AGE_MOUSE_RIGHT;
		handleKeyboard = true;
		handleMouse = true;
		handleController = true;
	}
}
