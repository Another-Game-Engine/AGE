#pragma once

#include <list>
#include <Utils/Dependency.hpp>
#include <glm/gtc/type_precision.hpp>
#include <Core/AgeInputs.hh>
#include <mutex>
#include <atomic>
#include <Utils/SpinLock.hpp>

namespace AGE
{

	class Input : public Dependency < Input >
	{
	private:
		std::list<AgeInputs>	_inputs;
		std::list<AgeKeys>		_keyInputs;

		std::atomic_int32_t _mousePosX;
		std::atomic_int32_t _mousePosY;
		std::atomic_int32_t _mouseDelX;
		std::atomic_int32_t _mouseDelY;
		std::atomic_int32_t _mouseWheelX;
		std::atomic_int32_t _mouseWheelY;

		AGE::SpinLock      _mutex;
	public:
		Input();
		virtual ~Input() { }

		void 				addInput(AgeInputs input);
		void 				removeInput(AgeInputs input);
		void 				addKeyInput(AgeKeys input);
		void 				removeKeyInput(AgeKeys input);
		void 				setMousePosition(glm::ivec2 const &pos, glm::ivec2 const &rel);
		void				setMouseWheel(glm::ivec2 const &delta);
		glm::ivec2  	    getMousePosition();
		glm::ivec2      	getMouseDelta();
		glm::ivec2      	getMouseWheel();
		bool 				getInput(AgeInputs input, bool handled = false);
		bool 				getKey(AgeKeys input, bool handled = false);

		void				sendMouseStateToIMGUI();
	};

}