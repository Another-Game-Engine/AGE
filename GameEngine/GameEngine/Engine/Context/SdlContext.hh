#pragma once

#include <SDL/SDL.h>
#include "context/IRenderContext.hh"
#include <Core/Input.hh>
#include <Utils/DependenciesInjector.hpp>
#include <Configuration.hpp>

#ifdef USE_IMGUI
#include <Utils/Age_Imgui.hpp>
#endif

class SdlContext : public IRenderContext
{
private:
	SDL_Window		*_window;
	SDL_GLContext	_glContext;
protected:
	bool _init(int mode);
public:
	SdlContext() { }
	virtual ~SdlContext() { }
	void swapContext()
	{
		SDL_GL_SwapWindow(_window);
	}

	void refreshInputs()
	{
		SDL_Event events;
		auto input = _dependencyManager.lock()->getInstance<Input>();
		std::lock_guard<std::mutex>(input->getMutex());
		input->clearInputs();
		while (SDL_PollEvent(&events))
		{
			if (events.type == SDL_KEYDOWN)
			{
				input->addKeyInput(events.key.keysym.sym);
#ifdef USE_IMGUI
				ImGuiIO& io = ImGui::GetIO();
				io.KeysDown[events.key.keysym.scancode] = true;
				//io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
				//io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
#endif
			}
			else if (events.type == SDL_KEYUP)
			{
#ifdef USE_IMGUI
				ImGuiIO& io = ImGui::GetIO();
				io.KeysDown[events.key.keysym.scancode] = false;
				//io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
				//io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
#endif
				input->removeKeyInput(events.key.keysym.sym);
			}
			else if (events.type == SDL_MOUSEBUTTONDOWN)
				input->addKeyInput(events.button.button);
			else if (events.type == SDL_MOUSEBUTTONUP)
				input->removeKeyInput(events.button.button);
			else if (events.type == SDL_MOUSEWHEEL)
				input->setMouseWheel(glm::i8vec2(events.wheel.x, events.wheel.y));
			else if (events.type == SDL_MOUSEMOTION)
				input->setMousePosition(glm::i8vec2(events.motion.x, events.motion.y), glm::i8vec2(events.motion.xrel, events.motion.yrel));
			else
				input->addInput(events.type);
		}
	}

	const glm::uvec2 &getScreenSize()
	{
		return _screenSize;
	}

	void setScreenSize(const glm::uvec2 &screenSize)
	{
		_screenSize = screenSize;
		SDL_SetWindowSize(_window, _screenSize.x, _screenSize.y);
		SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
};