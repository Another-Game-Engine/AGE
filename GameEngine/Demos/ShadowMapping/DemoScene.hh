#ifndef DEMOSCENE_HH_
# define DEMOSCENE_HH_

# include "Core/AScene.hh"
# include "Core/Engine.hh"

# include <MediaFiles/AMediaFile.hpp>
# include <Systems/MeshRenderSystem.h>
# include <Systems/CameraSystem.hpp>
# include <Systems/RotationForceSystem.hpp>
# include <Systems/TrackBallSystem.hpp>


class DemoScene : public AScene
{
public:
	DemoScene(std::weak_ptr<Engine> engine);
	virtual ~DemoScene();
	virtual bool userStart();
	virtual bool userUpdate(double time);
private:
	void initSytemeScene();
	void initRenderer();
	void loadResources();
};

#endif /*!DEMOSCENE_HH_*/