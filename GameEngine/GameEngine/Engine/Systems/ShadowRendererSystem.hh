#ifndef SHADOWRENDERERSYSTEM_HH_
# define SHADOWRENDERERSYSTEM_HH_

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include "System.h"
# include <Components/MeshRenderer.hh>
# include <Entities/EntityData.hh>
# include <Core/SceneManager.hh>

class ShadowRendererSystem : public System
{
public:
	ShadowRendererSystem(AScene *scene);
	virtual ~ShadowRendererSystem();
	void render(double time);

protected:
	GLuint _frameBuffer;
	uint32_t _height;
	uint32_t _width;
	GLuint _texture;
	EntityFilter _filter;
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual void initialize();

};

# include "ShadowRendererSystem.hpp"

#endif /*!SHADOWRENDERERSYSTEM_HPP_*/