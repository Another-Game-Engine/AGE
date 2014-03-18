#include "LightRenderingSystem.hh"

#include <Components\MeshRenderer.hh>
#include <Components\CameraComponent.hpp>
#include <Context\IRenderContext.hh>
#include <OpenGL\VertexBuffer.hh>

#include <glm/gtc/matrix_transform.hpp>

LightRenderingSystem::LightRenderingSystem(std::weak_ptr<AScene> scene) :
						System(scene),
						_pointLightFilter(scene),
						_spotLightFilter(scene),
						_meshRendererFilter(scene),
						_cameraFilter(scene),
						_spotShadowNbr(0),
						_pointShadowNbr(0),
						_shadowDimensions(1500, 1500),
						_idealIllum(0.3f),
						_adaptationSpeed(0.15f),
						_maxDarkImprovement(1.0f),
						_maxLightDiminution(0),
						_curFactor(1.0f),
						_targetFactor(1.0f),
						_useHDR(true),
						_useBloom(true),
						_bloomTextureSize(0),
						_bloomSigma(5.0f),
						_bloomGlare(1.0f),
						_bloomSpreading(2.0f)
{
}

LightRenderingSystem::~LightRenderingSystem()
{
	glDeleteTextures(1, &_spotShadowTextures);
	glDeleteTextures(1, &_bloomTexture);
	glDeleteFramebuffers(1, &_shadowsFbo);
}

void LightRenderingSystem::initialize()
{
	_scene.lock()->getInstance<Renderer>()->addShader("fboToScreenMultisampled", "../../Shaders/fboToScreen.vp", "../../Shaders/fboToScreenMultisampled.fp");
	_scene.lock()->getInstance<Renderer>()->addShader("fboToScreen", "../../Shaders/fboToScreen.vp", "../../Shaders/fboToScreen.fp");
	_quad.init(_scene);

	_pointLightFilter.requireComponent<Component::PointLight>();
	_spotLightFilter.requireComponent<Component::SpotLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();
	_cameraFilter.requireComponent<Component::CameraComponent>();

	_modulateRender.init("../../ComputeShaders/HighDynamicRange.kernel");
	_bloom.init("../../ComputeShaders/Bloom.kernel");

	auto materialBasic = _scene.lock()->getInstance<Renderer>()->getShader("MaterialBasic");

	// And lights uniform buffer
	_scene.lock()->getInstance<Renderer>()->addUniform("pointLightBuff")
		->init(materialBasic, "pointLightBuff", POINT_LIGHT_BUFF_SIZE);
	_scene.lock()->getInstance<Renderer>()->addUniform("spotLightBuff")
		->init(materialBasic, "spotLightBuff", SPOT_LIGHT_BUFF_SIZE);

	_scene.lock()->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "pointLightBuff", "pointLightBuff");
	_scene.lock()->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "spotLightBuff", "spotLightBuff");

	glEnable(GL_CULL_FACE);

	// shadows fbo and textures
	glGenTextures(1, &_spotShadowTextures);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _spotShadowTextures);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glGenFramebuffers(1, &_shadowsFbo);

	// Bloom texture
	glGenTextures(1, &_bloomTexture);
	glBindTexture(GL_TEXTURE_2D, _bloomTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void	LightRenderingSystem::updateLights(std::shared_ptr<OpenGLTools::UniformBuffer> perFrame)
{
	// Update the lights buffers
	auto		pointLightBuff = _scene.lock()->getInstance<Renderer>()->getUniform("pointLightBuff");
	auto		spotLightBuff = _scene.lock()->getInstance<Renderer>()->getUniform("spotLightBuff");
	auto		perLight = _scene.lock()->getInstance<Renderer>()->getUniform("PerLight");
	size_t		i = 0;
	size_t		shadowNbr = 0;

	_pointLightNbr = _pointLightFilter.getCollection().size();
	assert(_pointLightNbr <= MAX_LIGHT_NBR && "to many point lights");
	for (auto e : _pointLightFilter.getCollection())
	{
		_contiguousPointLights[i] = e->getComponent<Component::PointLight>()->lightData;
		_contiguousPointLights[i].positionPower.x = e->getGlobalTransform()[3].x;
		_contiguousPointLights[i].positionPower.y = e->getGlobalTransform()[3].y;
		_contiguousPointLights[i].positionPower.z = e->getGlobalTransform()[3].z;
		++i;
	}

	// Fill the buffer datas
	i = 0;
	shadowNbr = 0;
	_spotLightNbr = _spotLightFilter.getCollection().size();
	assert(_spotLightNbr <= MAX_LIGHT_NBR && "to many spot lights");
	for (auto e : _spotLightFilter.getCollection())
	{
		std::shared_ptr<Component::SpotLight>	spot = e->getComponent<Component::SpotLight>();
		spot->updateLightData();
		_contiguousSpotLights[i] = spot->lightData;
		if (_contiguousSpotLights[i].shadowId != -1)
			++shadowNbr;
		++i;
	}

	// Make the shadow texture array larger to fit all the lights textures in it
	if (_spotShadowNbr != shadowNbr)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, _spotShadowTextures);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, _shadowDimensions.x, _shadowDimensions.y, shadowNbr, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		_spotShadowNbr = shadowNbr;
	}

	_scene.lock()->getInstance<Renderer>()->getShader("ShadowDepth")->use();
	i = 0;
	shadowNbr = 0;
	glViewport(0, 0, _shadowDimensions.x, _shadowDimensions.y);
	// Update the lights shadowmaps

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);

	for (auto e : _spotLightFilter.getCollection())
	{
		SpotLightData	&spotLightData = e->getComponent<Component::SpotLight>()->lightData;

		if (spotLightData.shadowId != -1) // if the light has shadows, render the shadowmap
		{
			perLight->setUniform("lightVP", spotLightData.lightVP);
			perLight->flushChanges();

			glBindFramebuffer(GL_FRAMEBUFFER, _shadowsFbo);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _spotShadowTextures, 0, shadowNbr);
			
			glDrawBuffer(GL_NONE);
			glClear(GL_DEPTH_BUFFER_BIT);
			
			for (auto e : _meshRendererFilter.getCollection())
			{
				e->getComponent<Component::MeshRenderer>()->renderRaw();
			}

			_contiguousSpotLights[i].shadowId = shadowNbr;

			++shadowNbr;
		}
		++i;
	}

	// Transfert the uniform buffers to the GPU
	perFrame->setUniform("pointLightNbr", _pointLightNbr);
	perFrame->setUniform("spotLightNbr", _spotLightNbr);

	pointLightBuff->setBufferData(POINT_LIGHT_BUFF_SIZE, reinterpret_cast<char*>(_contiguousPointLights));
	spotLightBuff->setBufferData(SPOT_LIGHT_BUFF_SIZE, reinterpret_cast<char*>(_contiguousSpotLights));

	pointLightBuff->flushChanges();
	spotLightBuff->flushChanges();
}

void	LightRenderingSystem::mainUpdate(double time)
{
	auto renderer = _scene.lock()->getInstance<Renderer>();
	auto perFrame = renderer->getUniform("PerFrame");

	// set all the lights in the GPU uniform buffers
	updateLights(perFrame);

	// -------------------- Render -------------------- \\
	// For each camera
	for (auto c : _cameraFilter.getCollection())
	{
		auto viewport = c->getComponent<Component::CameraComponent>()->viewport;
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

		auto camera = c->getComponent<Component::CameraComponent>();
		// Set les uniforms du block PerFrame
		perFrame->setUniform("projection", camera->getProjection());
		perFrame->setUniform("view", camera->getLookAtTransform());
		perFrame->setUniform("time", static_cast<float>(_scene.lock()->getSystem<CameraSystem>()->getLifeTime()));

		perFrame->flushChanges();

		OpenGLTools::Framebuffer &fbo = c->getComponent<Component::CameraComponent>()->frameBuffer;

		if (fbo.isInit() == false)
		{
			fbo.init(_scene.lock()->getInstance<IRenderContext>()->getScreenSize(), 4);
			fbo.addTextureAttachment(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
			fbo.addTextureAttachment(GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT0);
			fbo.attachAll();
		}
		
		if (fbo.isMultisampled() && _downSampling.getSize() != fbo.getSize())
		{
			_downSampling.init(fbo.getSize(), 1);
			_downSampling.addTextureAttachment(GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT0);
			_downSampling.attachAll();
		}

		// First Pass
		computeCameraRender(fbo, perFrame);

		// If multisampled, downsample the texture
		if (fbo.isMultisampled())
		{
			// Down sample before post Fx
			_downSampling.bind();
			_quad.draw(fbo.getTextureAttachment(GL_COLOR_ATTACHMENT0), fbo.getSampleNbr(), fbo.getSize());
		}
		OpenGLTools::Framebuffer	&current = fbo.isMultisampled() ? _downSampling : fbo;

		// Compute HDR
		if (_useHDR)
			computeHdr(current);

		// Blit final result on back buffer:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_quad.draw(current.getTextureAttachment(GL_COLOR_ATTACHMENT0), current.getSampleNbr(), current.getSize());
	}
}

void		LightRenderingSystem::computeCameraRender(OpenGLTools::Framebuffer &camFbo,
													  std::shared_ptr<OpenGLTools::UniformBuffer> perFrame)
{
	auto renderer = _scene.lock()->getInstance<Renderer>();

	// ----------------------------------------------------
	camFbo.bind();

	// Z PrePass
	// ----------------------------------------------------
	glDrawBuffer(GL_NONE);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderer->getShader("depthOnly")->use();

	for (auto e : _meshRendererFilter.getCollection())
	{
		e->getComponent<Component::MeshRenderer>()->renderRaw();
	}

	// ----------------------------------------------------
	// Final Lightning pass
	// ----------------------------------------------------
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glDepthFunc(GL_LEQUAL);
	glColorMask(1, 1, 1, 1);

	GLuint		spotShadowMap = _spotShadowTextures;

	for (auto e : _meshRendererFilter.getCollection())
	{
		e->getComponent<Component::MeshRenderer>()->render([&](OpenGLTools::Shader &s)
		{
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D_ARRAY, spotShadowMap);
		});
	}
}

void		LightRenderingSystem::computeHdr(OpenGLTools::Framebuffer &camFbo)
{
	GLuint	colorTexture = camFbo.getTextureAttachment(GL_COLOR_ATTACHMENT0);
	// ----------------------------------------------------
	// HDR Pass
	// ----------------------------------------------------
	// Average colors:
	// ----------------------------------------------------
	glm::vec4	avgColor(0);

	glBindTexture(GL_TEXTURE_2D, colorTexture);

	glGenerateMipmap(GL_TEXTURE_2D);

	float	maxDimension = glm::max(static_cast<float>(camFbo.getSize().x), static_cast<float>(camFbo.getSize().y));
	int		mipMapNbr = static_cast<int>(glm::floor(glm::log2(maxDimension)));

	glGetTexImage(GL_TEXTURE_2D, mipMapNbr, GL_RGBA, GL_FLOAT, &avgColor);
	// ----------------------------------------------------
	// Modulate colors:
	// ----------------------------------------------------
	size_t		WORK_GROUP_SIZE = 16;
	glm::uvec2	groupNbr = glm::uvec2((camFbo.getSize().x + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE,
									  (camFbo.getSize().y + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE);

	float	avgIllumination = avgColor.r * 0.2126f + avgColor.g * 0.7152f + avgColor.b * 0.0722f;

	_targetFactor = glm::clamp(_idealIllum / avgIllumination, _maxLightDiminution, _maxDarkImprovement);

	_modulateRender.use();

	GLint		avgIllumLocation = glGetUniformLocation(_modulateRender.getId(), "factor");

	if (_curFactor != _targetFactor)
	{
		if (_curFactor < _targetFactor)
		{
			_curFactor += _adaptationSpeed * _scene.lock()->getInstance<Timer>()->getElapsed();
			if (_curFactor > _targetFactor)
				_curFactor = _targetFactor;
		}
		else
		{
			_curFactor -= _adaptationSpeed * _scene.lock()->getInstance<Timer>()->getElapsed();
			if (_curFactor < _targetFactor)
				_curFactor = _targetFactor;
		}
	}

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	// Set the uniform
	glUniform1f(avgIllumLocation, _curFactor);

	// Bind color texture to modulate
	glBindImageTexture(0, colorTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

	glDispatchCompute(groupNbr.x, groupNbr.y, 1);

	if (_useBloom)
	{
		// ----------------------------------------------------
		// Bloom Pass:
		// ----------------------------------------------------
		// Creating bloom texture
		// ----------------------------------------------------

		if (_bloomTextureSize != camFbo.getSize())
		{
			glBindTexture(GL_TEXTURE_2D, _bloomTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, camFbo.getSize().x, camFbo.getSize().y, 0, GL_RGBA, GL_FLOAT, NULL);
		}

		// ----------------------------------------------------
		_bloom.use();

		GLint		sigmaLocation = glGetUniformLocation(_bloom.getId(), "sigma");
		GLint		spreadingLocation = glGetUniformLocation(_bloom.getId(), "spreading");
		GLint		passLocation = glGetUniformLocation(_bloom.getId(), "pass");
		GLint		glareLocation = glGetUniformLocation(_bloom.getId(), "glareFactor");

		glUniform1f(sigmaLocation, _bloomSigma);
		glUniform1f(spreadingLocation, _bloomSpreading);
		glUniform1f(glareLocation, _bloomGlare);
		glUniform2i(passLocation, 1, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glBindImageTexture(1, _bloomTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glDispatchCompute(groupNbr.x, groupNbr.y, 1);

		glUniform2i(passLocation, 0, 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _bloomTexture);
		glBindImageTexture(1, colorTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glDispatchCompute(groupNbr.x, groupNbr.y, 1);

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
}
