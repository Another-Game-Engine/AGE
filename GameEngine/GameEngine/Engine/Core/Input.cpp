
#include "Input.hh"

Input::Input() :
	_mousePosition(0),
	_mouseDelta(0)
{
}

void 	Input::clearInputs()
{
	_inputs.clear();
	_mouseWheelDelta = _mouseDelta = glm::i8vec2(0);
}

void 	Input::addInput(int input)
{
	_inputs.push_back(input);
}

void 	Input::addKeyInput(int input)
{
	_keyInputs.push_back(input);
}

void 	Input::removeKeyInput(int input)
{
	_keyInputs.remove(input);
}

void 				Input::setMousePosition(glm::i8vec2 const &pos)
{
	if (_mousePosition != glm::i8vec2(0))
		_mouseDelta = _mousePosition - pos;
	_mousePosition = pos;
}

void				Input::setMouseWheel(glm::i8vec2 const &delta)
{
	_mouseWheelDelta = delta;
}

glm::i8vec2 const  	&Input::getMouseWheel()
{
	return (_mouseWheelDelta);
}

glm::i8vec2 const  	&Input::getMousePosition()
{
	return (_mousePosition);
}

glm::i8vec2 const  	&Input::getMouseDelta()
{
	return (_mouseDelta);
}

bool 	Input::getInput(int input, bool handled)
{
	std::list<int>::iterator		it = _inputs.begin();

	while (it != _inputs.end())
	{
		if (*it == input)
		{
			if (handled)
				_inputs.erase(it);
			return (true);
		}
		++it;
	}
	return (getKey(input, handled));
}

bool 	Input::getKey(int input, bool handled)
{
	std::list<int>::iterator		it = _keyInputs.begin();

	while (it != _keyInputs.end())
	{
		if (*it == input)
		{
			if (handled)
				_keyInputs.erase(it);
			return (true);
		}
		++it;
	}
	return (false);
}
