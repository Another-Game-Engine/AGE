
#ifndef   SDLCONTEXT_HH_
# define  SDLCONTEXT_HH_

#include <SDL/SDL.h>

#include "context/IRenderContext.hh"

class SdlContext : public IRenderContext
{
private:
	SDL_Window		*_window;
	SDL_GLContext	_glContext;
protected:
	virtual bool _init(int mode);
	virtual bool _update();
public:
  SdlContext() { }
  virtual ~SdlContext() { }
};

#endif