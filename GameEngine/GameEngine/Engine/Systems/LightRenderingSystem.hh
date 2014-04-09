#pragma once

#include <Components\PointLight.hh>
#include <Components\SpotLight.hh>

#include <Systems\System.h>
#include <Systems\CameraSystem.hpp>
#include <OpenGL\VertexManager.hh>
#include <OpenGL\Framebuffer.hh>
#include <Utils\DrawQuad.hh>

#include <vector>

#define		MAX_LIGHT_NBR	255
#define		POINT_LIGHT_BUFF_SIZE	(MAX_LIGHT_NBR * sizeof(PointLightData))
#define		SPOT_LIGHT_BUFF_SIZE	(MAX_LIGHT_NBR * sizeof(SpotLightData))

class LightRenderingSystem : public System
{
public:
	LightRenderingSystem(std::weak_ptr<AScene> &&scene);
	virtual ~LightRenderingSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual bool initialize();
	void drawSprites();

private:
	// Filters
	EntityFilter						_pointLightFilter;
	EntityFilter						_spotLightFilter;
	EntityFilter						_meshRendererFilter;
	EntityFilter						_cameraFilter;
	EntityFilter                        _spriteFilter;

	// Point lights uniform buffer
	PointLightData						_contiguousPointLights[MAX_LIGHT_NBR];
	unsigned int						_pointLightNbr;

	// Spot lights uniform buffer
	SpotLightData						_contiguousSpotLights[MAX_LIGHT_NBR];
	unsigned int						_spotLightNbr;

	void								updateLights(std::shared_ptr<OpenGLTools::UniformBuffer> perFrame);

	// Shadow Textures
	unsigned int						_spotShadowNbr;
	GLuint								_spotShadowTextures;
	unsigned int						_pointShadowNbr;
	GLuint								_pointShadowTextures;

	glm::uvec2							_shadowDimensions;

	// Shadow frame buffer

	GLuint								_shadowsFbo;

	void		computeCameraRender(OpenGLTools::Framebuffer &camFbo, std::shared_ptr<OpenGLTools::UniformBuffer> perFrame);
};

