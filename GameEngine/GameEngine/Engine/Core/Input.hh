
#ifndef INPUT_HH_
#define INPUT_HH_

#include <list>

#include "glm/gtc/type_precision.hpp"

class Input
{
private:
	std::list<int>	_inputs;
	std::list<int>	_keyInputs;
	glm::i8vec2		_mousePosition;
	glm::i8vec2		_mouseDelta;

public:
	Input();
	~Input() { }

	void 				clearInputs();
	void 				addInput(int input);
	void 				addKeyInput(int input);
	void 				removeKeyInput(int input);
	void 				setMousePosition(glm::i8vec2 const &pos);
	glm::i8vec2 const  	&getMousePosition();
	glm::i8vec2 const  	&getMouseDelta();
  	bool 				getInput(int input, bool handled = false);
  	bool 				getKey(int input, bool handled = false);
};

#endif