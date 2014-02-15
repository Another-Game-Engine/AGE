#pragma once

#include <Components\PointLight.hh>

#include <Systems\System.h>
#include <Systems\CameraSystem.hpp>
#include <OpenGL\VertexManager.hh>
#include <OpenGL\ComputeShader.hh>
#include <OpenGL\Framebuffer.hh>

#include <vector>

class LightRenderingSystem : public System
{
public:
	LightRenderingSystem(AScene *scene);
	virtual ~LightRenderingSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual void initialize();

	void		useHDR(bool use) { _useHDR = use; }
	void		setHDRIdealIllumination(float idealIllum) { _idealIllum = idealIllum; }
	void		setHDRAdaptationSpeed(float adaptSpeed) { _adaptationSpeed = adaptSpeed; }
	void		setHDRMaxDarkImprovement(float maxDarkImprovement) { _maxDarkImprovement = maxDarkImprovement; }
	void		setMaxLightDiminution(float maxLightDiminution) { _maxLightDiminution = maxLightDiminution; }

private:
	// Filters
	EntityFilter				_lightFilter;
	EntityFilter				_meshRendererFilter;
	EntityFilter				_cameraFilter;

	// Light Buffer
	GLuint								_lights;
	std::vector<ContiguousLight>		_contiguousLights;

	// HDR Utils
	// ----------------------------------------
	// HDR parameters
	float							_idealIllum;
	float							_adaptationSpeed;
	float							_maxDarkImprovement;
	float							_maxLightDiminution;

	// HDR Compute shader
	OpenGLTools::ComputeShader		_averageColor;
	OpenGLTools::ComputeShader		_modulateRender;

	// HDR Average color
	GLuint							_avgColors;

	// HDR factors
	float							_curFactor;
	float							_targetFactor;

	// Use HDR
	bool							_useHDR;

	void		computeHdr(OpenGLTools::Framebuffer &camFbo);
	void		computeCameraRender(OpenGLTools::Framebuffer &camFbo, OpenGLTools::UniformBuffer *perFrame);
};

