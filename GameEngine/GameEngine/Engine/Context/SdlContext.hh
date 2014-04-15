
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
	virtual void _setScreenSize(const glm::uvec2 &screenSize);
	virtual bool _start(int mode);
public:
  SdlContext() { }
  virtual ~SdlContext() { }

  virtual void    updateEvents(Input &input) const;
  virtual void 	  flush() const;
  virtual void    stop() const;
};

#endif