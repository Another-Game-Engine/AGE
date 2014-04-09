#pragma once
#include <Systems/System.h>
#include <Utils/DrawQuad.hh>
#include <utility>

class PostFxSystem : public System
{
public:
	PostFxSystem(std::weak_ptr<AScene> scene);
	virtual ~PostFxSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual bool initialize();

	void		useBloom(bool use) { _useBloom = use; }
	void		setBloomSigma(float sigma) { _bloomSigma = sigma; }
	void		setBloomGlare(float glare) { _bloomGlare = glare; }
	void		setBloomMipmap(uint32_t mipmap) { _bloomMipmap = mipmap; }
	void		useHDR(bool use) { _useHDR = use; }
	void		setHDRIdealIllumination(float idealIllum) { _idealIllum = idealIllum; }
	void		setHDRAdaptationSpeed(float adaptSpeed) { _adaptationSpeed = adaptSpeed; }
	void		setHDRMaxDarkImprovement(float maxDarkImprovement) { _maxDarkImprovement = maxDarkImprovement; }
	void		setHDRMaxLightDiminution(float maxLightDiminution) { _maxLightDiminution = maxLightDiminution; }

private:
	EntityFilter						_cameraFilter;

	// DownSample quad
	DrawQuad						_quad;

	// HDR Utils
	// ----------------------------------------
	// HDR parameters
	float							_idealIllum;
	float							_adaptationSpeed;
	float							_maxDarkImprovement;
	float							_maxLightDiminution;

	// HDR Compute shader
	OpenGLTools::Shader		_modulateRender;
	OpenGLTools::Shader		_bloom;

	// HDR factors
	float							_curFactor;
	float							_targetFactor;

	// Use HDR
	bool							_useHDR;

	bool							_useBloom;

	glm::uvec2						_bloomTextureSize;
	GLuint							_bloomTexture;
	float							_bloomSigma;
	float							_bloomGlare;
	uint32_t						_bloomMipmap;

	void		computeHdr(OpenGLTools::Framebuffer &camFbo);
	void		computeBloom(OpenGLTools::Framebuffer &camFbo);
};

