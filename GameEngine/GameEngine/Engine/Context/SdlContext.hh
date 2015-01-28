#pragma once

#include "context/IRenderContext.hh"

class SDL_Window;

namespace AGE
{
	class SdlContext : public IRenderContext
	{
	private:
		SDL_Window		*_window;
		void	*_glContext;
	protected:
		bool _init(int mode);
	public:
		SdlContext();
		virtual ~SdlContext();
		void swapContext();
		void refreshInputs();
		const glm::uvec2 &getScreenSize();
		void setScreenSize(const glm::uvec2 &screenSize);
	};
}