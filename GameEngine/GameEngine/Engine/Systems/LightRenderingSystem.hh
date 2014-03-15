#pragma once

#include <Components\PointLight.hh>
#include <Components\SpotLight.hh>

#include <Systems\System.h>
#include <Systems\CameraSystem.hpp>
#include <OpenGL\VertexManager.hh>
#include <OpenGL\ComputeShader.hh>
#include <OpenGL\Framebuffer.hh>
#include <Utils\DrawQuad.hh>

#include <vector>

#define		MAX_LIGHT_NBR	255
#define		POINT_LIGHT_BUFF_SIZE	(MAX_LIGHT_NBR * sizeof(PointLightData))
#define		SPOT_LIGHT_BUFF_SIZE	(MAX_LIGHT_NBR * sizeof(SpotLightData))

class LightRenderingSystem : public System
{
public:
	LightRenderingSystem(AScene *scene);
	virtual ~LightRenderingSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual void initialize();

	void		useBloom(bool use) { _useBloom = use; }
	void		setBloomSigma(float sigma) { _bloomSigma = sigma; }
	void		setBloomGlare(float glare) { _bloomGlare = glare; }
	void		setBloomSpreading(float spread) { _bloomSpreading = glm::max(spread, 1.0f); }
	void		useHDR(bool use) { _useHDR = use; }
	void		setHDRIdealIllumination(float idealIllum) { _idealIllum = idealIllum; }
	void		setHDRAdaptationSpeed(float adaptSpeed) { _adaptationSpeed = adaptSpeed; }
	void		setHDRMaxDarkImprovement(float maxDarkImprovement) { _maxDarkImprovement = maxDarkImprovement; }
	void		setHDRMaxLightDiminution(float maxLightDiminution) { _maxLightDiminution = maxLightDiminution; }

private:
	// Filters
	EntityFilter						_pointLightFilter;
	EntityFilter						_spotLightFilter;
	EntityFilter						_meshRendererFilter;
	EntityFilter						_cameraFilter;

	// Point lights uniform buffer
	PointLightData						_contiguousPointLights[MAX_LIGHT_NBR];
	unsigned int						_pointLightNbr;

	// Spot lights uniform buffer
	SpotLightData						_contiguousSpotLights[MAX_LIGHT_NBR];
	unsigned int						_spotLightNbr;

	void								updateLights(OpenGLTools::UniformBuffer *perFrame);

	// Shadow Textures
	unsigned int						_spotShadowNbr;
	GLuint								_spotShadowTextures;
	unsigned int						_pointShadowNbr;
	GLuint								_pointShadowTextures;

	glm::uvec2							_shadowDimensions;

	// Shadow frame buffer

	GLuint								_shadowsFbo;

	// HDR Utils
	// ----------------------------------------
	// HDR parameters
	float							_idealIllum;
	float							_adaptationSpeed;
	float							_maxDarkImprovement;
	float							_maxLightDiminution;

	// HDR Compute shader
	OpenGLTools::ComputeShader		_modulateRender;
	OpenGLTools::ComputeShader		_bloom;

	// HDR factors
	float							_curFactor;
	float							_targetFactor;

	// Use HDR
	bool							_useHDR;

	// Final render quad
	DrawQuad						_quad;

	OpenGLTools::Framebuffer		_downSampling;

	bool							_useBloom;

	glm::uvec2						_bloomTextureSize;
	GLuint							_bloomTexture;
	float							_bloomSigma;
	float							_bloomGlare;
	float							_bloomSpreading;

	void		computeHdr(OpenGLTools::Framebuffer &camFbo);
	void		computeCameraRender(OpenGLTools::Framebuffer &camFbo, OpenGLTools::UniformBuffer *perFrame);
};

