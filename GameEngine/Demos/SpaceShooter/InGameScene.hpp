#ifndef		__INGAME_SCENE_HPP__
#define		__INGAME_SCENE_HPP__

#include <Core/Engine.hh>

class InGameScene : public AScene
{
public:
	InGameScene(Engine &engine)
		: AScene(engine)
	{}

	virtual ~InGameScene(void)
	{}

	virtual bool 			userStart()
	{
		return true;
	}

	virtual bool 			userUpdate(double time)
	{
		if (_engine.getInstance<Input>().getInput(SDLK_ESCAPE) ||
			_engine.getInstance<Input>().getInput(SDL_QUIT))
			return (false);
		return (true);
	}
};

#endif //__INGAME_SCENE_HPP__