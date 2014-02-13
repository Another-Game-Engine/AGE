#pragma once

#include <Components\PointLight.hh>

#include <Systems\System.h>
#include <OpenGL\VertexManager.hh>
#include <OpenGL\ComputeShader.hh>

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

private:
	// Filters
	EntityFilter				_lightFilter;
	EntityFilter				_meshRendererFilter;

	void						initFrameBuffer();
	// Frame Buffer
	GLuint						_frameBuffer;
	// Render textures
	GLuint						_depthTexture;
	GLuint						_colorTexture;

	// Quad to draw full screen
	void						initQuad();
		
	Vertice<2>					_quad;
	VertexManager<2>			*_vertexManager;

	// Light Buffer
	GLuint						_lights;
	std::vector<ContiguousLight>		_contiguousLights;

	// HDR Compute shader
	OpenGLTools::ComputeShader		_averageColor;
	OpenGLTools::ComputeShader		_modulateRender;

	// HDR Average color
	GLuint							_avgColors;

	// HDR factors
	float							_curFactor;
	float							_targetFactor;
};

