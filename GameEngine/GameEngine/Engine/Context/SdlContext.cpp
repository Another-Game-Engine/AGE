#include "context/SdlContext.hh"

bool    SdlContext::start(unsigned int swidth, unsigned int sheight, const char *name)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0 ||
		(_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		swidth, sheight, SDL_WINDOW_OPENGL)) == NULL ||
		(_glContext = SDL_GL_CreateContext(_window)) == NULL)
			return (false);
	return (true);
}

void    SdlContext::updateEvents(Input &input) const
{
  SDL_Event events;

  while (SDL_PollEvent(&events))
  {
    if (events.type == SDL_KEYDOWN)
      input.addKeyInput(events.key.keysym.sym);
    if (events.type == SDL_KEYUP)
      input.removeKeyInput(events.key.keysym.sym);
    else if (events.type == SDL_MOUSEMOTION)
      input.setMousePosition(glm::i8vec2(events.motion.x, events.motion.x));
    else
      input.addInput(events.type);
  }
}

void    SdlContext::flush() const
{
	SDL_GL_SwapWindow(_window);
}

void    SdlContext::stop() const
{
  SDL_Quit();
}
