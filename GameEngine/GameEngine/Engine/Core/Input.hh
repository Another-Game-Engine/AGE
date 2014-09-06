#pragma once

#include <list>
#include <Utils/Dependency.hpp>
#include "glm/gtc/type_precision.hpp"
#include <mutex>
#include <atomic>

class Input : public Dependency<Input>
{
private:
	std::list<int>	_inputs;
	std::list<int>	_keyInputs;

	std::atomic_int32_t _mousePosX;
	std::atomic_int32_t _mousePosY;
	std::atomic_int32_t _mouseDelX;
	std::atomic_int32_t _mouseDelY;
	std::atomic_int32_t _mouseWheelX;
	std::atomic_int32_t _mouseWheelY;

	std::mutex      _mutex;
public:
	Input();
	virtual ~Input() { }

	void 				clearInputs();
	void 				addInput(int input);
	void 				addKeyInput(int input);
	void 				removeKeyInput(int input);
	void 				setMousePosition(glm::i8vec2 const &pos, glm::i8vec2 const &rel);
	void				setMouseWheel(glm::i8vec2 const &delta);
	glm::i8vec2 const  	&getMousePosition();
	glm::i8vec2 const  	&getMouseDelta();
	glm::i8vec2 const  	&getMouseWheel();
  	bool 				getInput(int input, bool handled = false);
  	bool 				getKey(int input, bool handled = false);
	inline std::mutex   &getMutex(){ return _mutex; }
};