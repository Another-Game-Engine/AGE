
#ifndef   SDLCONTEXT_HH_
# define  SDLCONTEXT_HH_

#include <SDL/SDL.h>

#include "context/IRenderContext.hh"

class SdlContext : public IRenderContext
{
private:
	SDL_Window		*_window;
	SDL_GLContext	_glContext;
public:
  SdlContext() { }
  virtual ~SdlContext() { }

  virtual bool    start(int mode, unsigned int swidth, unsigned int sheight, const char *name);
  virtual void    updateEvents(Input &input) const;
  virtual void 	  flush() const;
  virtual void    stop() const;
};

#endif