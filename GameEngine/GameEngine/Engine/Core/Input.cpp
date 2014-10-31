
#include "Input.hh"

Input::Input() :
	_mousePosX(0),
	_mousePosY(0),
	_mouseDelX(0),
	_mouseDelY(0),
	_mouseWheelX(0),
	_mouseWheelY(0)
{
}

void 	Input::clearInputs()
{
	std::lock_guard<std::mutex> lock(_mutex);
	_inputs.clear();
	_mousePosX = _mousePosY = _mouseWheelX = _mouseWheelY = _mouseDelX = _mouseDelY = 0;
}

void 	Input::addInput(int input)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_inputs.push_back(input);
}

void 	Input::addKeyInput(int input)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_keyInputs.push_back(input);
}

void 	Input::removeKeyInput(int input)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_keyInputs.remove(input);
}

void 				Input::setMousePosition(glm::i8vec2 const &pos, glm::i8vec2 const &rel)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_mouseDelX = rel.x;
	_mouseDelY = rel.y;
	_mousePosX = rel.x;
	_mousePosY = rel.y;
}

void				Input::setMouseWheel(glm::i8vec2 const &delta)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_mouseWheelX = delta.x;
	_mouseWheelY = delta.y;
}

glm::i8vec2   	    Input::getMouseWheel()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return glm::i8vec2(_mouseWheelX, _mouseWheelY);
}

glm::i8vec2      	Input::getMousePosition()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return glm::i8vec2(_mousePosX, _mousePosY);
}

glm::i8vec2       	Input::getMouseDelta()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return glm::i8vec2(_mouseDelX, _mouseDelY);
}

bool 	Input::getInput(int input, bool handled)
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
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
	}
	return (getKey(input, handled));
}

bool 	Input::getKey(int input, bool handled)
{
	std::lock_guard<std::mutex> lock(_mutex);
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
