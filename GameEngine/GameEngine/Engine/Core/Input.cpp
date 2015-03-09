
#include <Core/Input.hh>
#include <Configuration.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>

#ifdef USE_IMGUI
#include <Utils/Age_Imgui.hpp>
#endif

namespace AGE
{

	Input::Input() :
		_mousePosX(0),
		_mousePosY(0),
		_mouseDelX(0),
		_mouseDelY(0),
		_mouseWheelX(0),
		_mouseWheelY(0)
	{
	}

	void 	Input::addInput(AgeInputs input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_inputs.push_back(input);
	}

	void 	Input::removeInput(AgeInputs input)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_inputs.remove(input);
	}

	void 	Input::addKeyInput(AgeKeys input)
	{
		_mutex.lock();
		_keyInputs.push_back(input);
		_mutex.unlock();
#ifdef USE_IMGUI
		GetMainThread()->getQueue()->emplaceTask<ImGuiKeyEvent>(input, true);
#endif
	}

	void 	Input::removeKeyInput(AgeKeys input)
	{
		_mutex.lock();
		_keyInputs.remove(input);
		_mutex.unlock();
#ifdef USE_IMGUI
		GetMainThread()->getQueue()->emplaceTask<ImGuiKeyEvent>(input, false);
#endif
	}

	void 				Input::setMousePosition(glm::ivec2 const &pos, glm::ivec2 const &rel)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_mouseDelX = rel.x;
		_mouseDelY = rel.y;
		_mousePosX = pos.x;
		_mousePosY = pos.y;
	}

	void				Input::setMouseWheel(glm::ivec2 const &delta)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_mouseWheelX = delta.x;
		_mouseWheelY = delta.y;
	}

	glm::ivec2   	    Input::getMouseWheel()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return glm::ivec2(_mouseWheelX, _mouseWheelY);
	}

	glm::ivec2      	Input::getMousePosition()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return glm::ivec2(_mousePosX, _mousePosY);
	}

	glm::ivec2       	Input::getMouseDelta()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return glm::ivec2(_mouseDelX, _mouseDelY);
	}

	bool 	Input::getInput(AgeInputs input, bool handled)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		std::list<AgeInputs>::iterator it = _inputs.begin();

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

	bool 	Input::getKey(AgeKeys input, bool handled)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		std::list<AgeKeys>::iterator it = _keyInputs.begin();

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

	void Input::sendMouseStateToIMGUI()
	{
#ifdef USE_IMGUI
		glm::ivec2 mousePosition(_mousePosX, _mousePosY);
		GetMainThread()->getQueue()->emplaceTask<ImGuiMouseStateEvent>(mousePosition,
			getInput(AGE_MOUSE_LEFT),
			getInput(AGE_MOUSE_MIDDLE),
			getInput(AGE_MOUSE_RIGHT));
#endif
	}

}