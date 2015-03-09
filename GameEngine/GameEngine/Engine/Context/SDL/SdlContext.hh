#pragma once

#include <context/IRenderContext.hh>

class SDL_Window;

namespace AGE
{
	class SdlContext : public IRenderContext
	{
	public:
		SdlContext();
		virtual ~SdlContext();

		virtual void swapContext();
		virtual void refreshInputs();
		virtual const glm::uvec2 &getScreenSize();
		virtual void setScreenSize(const glm::uvec2 &screenSize);

	protected:
		bool _init(int mode);

	private:
		SDL_Window		*_window;
		void	*_glContext;
	};
}