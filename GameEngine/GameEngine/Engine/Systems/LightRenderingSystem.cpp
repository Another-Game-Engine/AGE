#include "LightRenderingSystem.hh"

#include <Components\MeshRenderer.hh>
#include <Components\CameraComponent.hpp>
#include <Context\IRenderContext.hh>
#include <OpenGL\VertexBuffer.hh>

#include <glm/gtc/matrix_transform.hpp>

LightRenderingSystem::LightRenderingSystem(AScene *scene) :
						System(scene),
						_pointLightFilter(scene),
						_spotLightFilter(scene),
						_meshRendererFilter(scene),
						_cameraFilter(scene),
						_idealIllum(0.3f),
						_adaptationSpeed(0.15f),
						_maxDarkImprovement(1.0f),
						_maxLightDiminution(0),
						_curFactor(1.0f),
						_targetFactor(1.0f),
						_useHDR(true)
{
}

LightRenderingSystem::~LightRenderingSystem()
{
	glDeleteBuffers(1, &_avgColors);
}

void LightRenderingSystem::initialize()
{
	_quad.init(_scene->getEngine());

	_pointLightFilter.requireComponent<Component::PointLight>();
	_spotLightFilter.requireComponent<Component::SpotLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();
	_cameraFilter.requireComponent<Component::CameraComponent>();

	_averageColor.init("./ComputeShaders/AverageColorFirstPass.kernel");
	_modulateRender.init("./ComputeShaders/HighDynamicRange.kernel");

	// And lights uniform buffer
	_scene->getInstance<Renderer>()->addUniform("pointLightBuff")
		.init(POINT_LIGHT_BUFF_SIZE);
	_scene->getInstance<Renderer>()->addUniform("spotLightBuff")
		.init(SPOT_LIGHT_BUFF_SIZE);

	_scene->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "pointLightBuff", "pointLightBuff");
	_scene->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "spotLightBuff", "spotLightBuff");

	// Gen average color buffer
	glGenBuffers(1, &_avgColors);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _avgColors);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void	LightRenderingSystem::updateLights(OpenGLTools::UniformBuffer *perFrame)
{
	// Update the lights buffers
	auto		pointLightBuff = _scene->getInstance<Renderer>()->getUniform("pointLightBuff");
	auto		spotLightBuff = _scene->getInstance<Renderer>()->getUniform("spotLightBuff");
	size_t		i = 0;

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
	i = 0;
	_spotLightNbr = _spotLightFilter.getCollection().size();
	assert(_spotLightNbr <= MAX_LIGHT_NBR && "to many spot lights");
	for (auto e : _spotLightFilter.getCollection())
	{
		assert(i < MAX_LIGHT_NBR && "to many point lights");
		_contiguousSpotLights[i] = e->getComponent<Component::SpotLight>()->lightData;
		_contiguousSpotLights[i].positionPower.x = e->getGlobalTransform()[3].x;
		_contiguousSpotLights[i].positionPower.y = e->getGlobalTransform()[3].y;
		_contiguousSpotLights[i].positionPower.z = e->getGlobalTransform()[3].z;
		++i;
	}

	perFrame->setUniform("pointLightNbr", _pointLightNbr);
	perFrame->setUniform("spotLightNbr", _spotLightNbr);

	pointLightBuff->setBufferData(POINT_LIGHT_BUFF_SIZE, reinterpret_cast<char*>(_contiguousPointLights));
	spotLightBuff->setBufferData(SPOT_LIGHT_BUFF_SIZE, reinterpret_cast<char*>(_contiguousSpotLights));

	pointLightBuff->flushChanges();
	spotLightBuff->flushChanges();
	// Update the lights shadowmaps
	for (auto e : _spotLightFilter.getCollection())
	{
		SpotLightData	&spotLightData = e->getComponent<Component::SpotLight>()->lightData;

		if (spotLightData.hasShadow) // if the light has shadows, render the shadowmap
		{
			// Render raw from camera point of view to a framebuffer
		}
	}
}

void	LightRenderingSystem::mainUpdate(double time)
{
	auto renderer = _scene->getInstance<Renderer>();
	auto perFrame = renderer->getUniform("PerFrame");

	// set all the lights in the GPU uniform buffers
	updateLights(perFrame);

	// -------------------- Render -------------------- \\
	// For each camera
	for (auto c : _cameraFilter.getCollection())
	{
		auto camera = c->getComponent<Component::CameraComponent>();
		// Set les uniforms du block PerFrame
		perFrame->setUniform("projection", camera->getProjection());
		perFrame->setUniform("view", camera->getLookAtTransform());
		perFrame->setUniform("time", static_cast<float>(_scene->getSystem<CameraSystem>()->getLifeTime()));

		perFrame->flushChanges();

		OpenGLTools::Framebuffer &fbo = c->getComponent<Component::CameraComponent>()->frameBuffer;

		if (fbo.isInit() == false)
		{
			fbo.init(_scene->getInstance<IRenderContext>()->getScreenSize(), 4);
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
		auto viewport = c->getComponent<Component::CameraComponent>()->viewport;
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		_quad.draw(current.getTextureAttachment(GL_COLOR_ATTACHMENT0), current.getSampleNbr(), current.getSize());
	}
}

void		LightRenderingSystem::computeCameraRender(OpenGLTools::Framebuffer &camFbo,
													  OpenGLTools::UniformBuffer *perFrame)
{
	auto renderer = _scene->getInstance<Renderer>();

	// ----------------------------------------------------
	camFbo.bind();

	// Z PrePass
	// ----------------------------------------------------
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	renderer->getShader("depthOnly")->use();

	for (auto e : _meshRendererFilter.getCollection())
	{
		e->getComponent<Component::MeshRenderer>()->renderRaw();
	}

	// ----------------------------------------------------
	// Final Lightning pass
	// ----------------------------------------------------
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glColorMask(1, 1, 1, 1);

	// shaders that needs light buffer
	GLuint		materialBasicId = -1;
	GLuint		earthId = -1;
	GLuint		bumpId = -1;

	auto		materialBasic = renderer->getShader("MaterialBasic");
	auto		earth = renderer->getShader("earth");
	auto		bump = renderer->getShader("bump");

	if (materialBasic != NULL)
		materialBasicId = materialBasic->getId();
	if (earth != NULL)
		earthId = earth->getId();
	if (bump != NULL)
		bumpId = bump->getId();
	for (auto e : _meshRendererFilter.getCollection())
	{
		e->getComponent<Component::MeshRenderer>()->render();
	}
}

void		LightRenderingSystem::computeHdr(OpenGLTools::Framebuffer &camFbo)
{
	GLuint	colorTexture = camFbo.getTextureAttachment(GL_COLOR_ATTACHMENT0);
	GLenum	textureType = camFbo.isMultisampled() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	// ----------------------------------------------------
	// HDR Pass
	// ----------------------------------------------------
	// TODO : Try to pack this in one pass
	// ----------------------------------------------------
	// Average colors:
	// ----------------------------------------------------
	glm::vec4	avgColor(0);

	_averageColor.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(textureType, colorTexture);

	glGenerateMipmap(GL_TEXTURE_2D);

	float	maxDimension = glm::max(static_cast<float>(camFbo.getSize().x), static_cast<float>(camFbo.getSize().y));
	int		mipMapNbr = static_cast<int>(glm::floor(glm::log2(maxDimension)));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, mipMapNbr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapNbr);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _avgColors);

	glDispatchCompute(1, 1, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _avgColors);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 4 * sizeof(float), glm::value_ptr(avgColor));
	// ----------------------------------------------------
	// Modulate colors:
	// ----------------------------------------------------
	size_t		WORK_GROUP_SIZE = 16;
	glm::uvec2	groupNbr = glm::uvec2((camFbo.getSize().x + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE,
									  (camFbo.getSize().y + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE);

	float	avgIllumination = avgColor.r * 0.2126f + avgColor.g * 0.7152f + avgColor.b * 0.0722f;

	_targetFactor = glm::clamp(_idealIllum / avgIllumination, _maxLightDiminution, _maxDarkImprovement);

	_modulateRender.use();

	GLint		colorBufferSizeLocation = glGetUniformLocation(_modulateRender.getId(), "colorBufferSize");
	GLint		avgIllumLocation = glGetUniformLocation(_modulateRender.getId(), "factor");

	if (_curFactor != _targetFactor)
	{
		if (_curFactor < _targetFactor)
		{
			_curFactor += _adaptationSpeed * _scene->getInstance<Timer>()->getElapsed();
			if (_curFactor > _targetFactor)
				_curFactor = _targetFactor;
		}
		else
		{
			_curFactor -= _adaptationSpeed * _scene->getInstance<Timer>()->getElapsed();
			if (_curFactor < _targetFactor)
				_curFactor = _targetFactor;
		}
	}

	// Set the uniforms
	glUniform2uiv(colorBufferSizeLocation, 1, glm::value_ptr(camFbo.getSize()));
	glUniform1f(avgIllumLocation, _curFactor);

	// Bind color texture to modulate
	glBindImageTexture(0, colorTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

	glDispatchCompute(groupNbr.x, groupNbr.y, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
