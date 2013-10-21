
#include "Input.hh"

Input::Input() :
	_mousePosition(0),
	_mouseDelta(0)
{
}

void 	Input::clearInputs()
{
	_inputs.clear();
}

void 	Input::addInput(int input)
{
	_inputs.push_back(input);
}

void 				Input::setMousePosition(glm::i8vec2 const &pos)
{
	if (_mousePosition != glm::i8vec2(0))
		_mouseDelta = _mousePosition - pos;
	_mousePosition = pos;
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
	return (false);
}
