#include "LightRenderingSystem.hh"

#include <Components\MeshRenderer.hh>
#include <Components\CameraComponent.hpp>
#include <Context\IRenderContext.hh>
#include <OpenGL\VertexBuffer.hh>

#include <glm/gtc/matrix_transform.hpp>

LightRenderingSystem::LightRenderingSystem(AScene *scene) :
						System(scene),
						_lightFilter(scene),
						_meshRendererFilter(scene),
						_cameraFilter(scene),
						_lights(0),
						_idealIllum(0.3f),
						_adaptationSpeed(0.15f),
						_maxDarkImprovement(1.0f),
						_maxLightDiminution(0),
						_curFactor(1.0f),
						_targetFactor(1.0f),
						_useHDR(true),
						_oldBuffSize(0),
						_avgBuffer(NULL)
{
}

LightRenderingSystem::~LightRenderingSystem()
{
	glDeleteBuffers(1, &_lights);
	glDeleteBuffers(1, &_avgColors);
	delete	_vertexManager;
	if (_avgBuffer != NULL)
		delete[] _avgBuffer;
}

void LightRenderingSystem::initialize()
{
	_lightFilter.requireComponent<Component::PointLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();
	_cameraFilter.requireComponent<Component::CameraComponent>();

	initQuad();

	_averageColor.init("./ComputeShaders/AverageColorFirstPass.kernel");
	_modulateRender.init("./ComputeShaders/HighDynamicRange.kernel");

	// Gen light buffer
	glGenBuffers(1, &_lights);
	// Gen average color buffer
	glGenBuffers(1, &_avgColors);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _avgColors);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
}

void LightRenderingSystem::mainUpdate(double time)
{
	auto renderer = _scene->getInstance<Renderer>();
	auto perFrame = renderer->getUniform("PerFrame");
	int lightNbr = _lightFilter.getCollection().size();

	perFrame->setUniform("lightNbr", lightNbr);
	perFrame->flushChanges(); 
	// Create the contiguous light buffer
	if (_contiguousLights.size() != lightNbr)
	{
		_contiguousLights.clear();
		for (auto e : _lightFilter.getCollection())
		{
			_contiguousLights.push_back(e->getComponent<Component::PointLight>()->lightData);
			_contiguousLights.back().positionPower.x = e->getGlobalTransform()[3].x;
			_contiguousLights.back().positionPower.y = e->getGlobalTransform()[3].y;
			_contiguousLights.back().positionPower.z = e->getGlobalTransform()[3].z;
		}
	}
	else // Or just update it
	{
		size_t		i = 0;

		for (auto e : _lightFilter.getCollection())
		{
			_contiguousLights[i] = e->getComponent<Component::PointLight>()->lightData;
			_contiguousLights[i].positionPower.x = e->getGlobalTransform()[3].x;
			_contiguousLights[i].positionPower.y = e->getGlobalTransform()[3].y;
			_contiguousLights[i].positionPower.z = e->getGlobalTransform()[3].z;
			++i;
		}
	}
	// Fill light buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, _lights);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _lights);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightNbr * sizeof(ContiguousLight), _contiguousLights.data(), GL_DYNAMIC_DRAW);
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
			fbo.init(_scene->getInstance<IRenderContext>()->getScreenSize());
			fbo.addTextureAttachment(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
			fbo.addTextureAttachment(GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT0);
			fbo.attachAll();
		}

		computeCameraRender(fbo, perFrame);
		// Render final quad on screen:
		auto viewport = c->getComponent<Component::CameraComponent>()->viewport;
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);

		perFrame->setUniform("view", glm::mat4(1));
		perFrame->setUniform("projection", glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
		perFrame->flushChanges();

		// Write on default FrameBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Rebind the default FrameBuffer
		glDisable(GL_DEPTH_TEST);

		renderer->getShader("fboToScreen")->use();

		// Bind texture of the final render
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo.getTextureAttachment(GL_COLOR_ATTACHMENT0));

		_quad.draw();
	}
}

void	LightRenderingSystem::initQuad()
{
	std::array<Attribute, 2> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 2),
		Attribute(GL_FLOAT, sizeof(float), 2)
	};
	_vertexManager = new VertexManager<2>(param);
	_vertexManager->init();

	// Init the Quad:
	// ------------------------------------
	// x,y vertex positions
	float quadPos[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0, 1.0,
		1.0, 1.0,
		-1.0, 1.0,
		-1.0, -1.0
	};

	// per-vertex texture coordinates
	float quadUvs[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0
	};

	unsigned int indice[] = { 0, 1, 2, 3, 4, 5 };

	std::array<Data, 2> data =
	{
		Data(6 * 2 * sizeof(float), quadPos),
		Data(6 * 2 * sizeof(float), quadUvs)
	};
	Data indicesData(6 * sizeof(unsigned int), indice);
	_quad = Vertice<2>(6, data, &indicesData);
	_vertexManager->addVertice(_quad);
	// ------------------------------------
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

	GLuint		localLightBuffId = _lights;

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
		e->getComponent<Component::MeshRenderer>()->render(
			[localLightBuffId, materialBasicId, earthId, bumpId](OpenGLTools::Shader &s)
		{
			if (s.getId() == earthId || s.getId() == materialBasicId || s.getId() == bumpId)
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, localLightBuffId);
			}
		}
		);
	}

	if (_useHDR)
		computeHdr(camFbo);
}

void		LightRenderingSystem::computeHdr(OpenGLTools::Framebuffer &camFbo)
{
	GLuint	colorTexture = camFbo.getTextureAttachment(GL_COLOR_ATTACHMENT0);
	// ----------------------------------------------------
	// HDR Pass
	// ----------------------------------------------------
	// ----------------------------------------------------
	// Average colors:
	// ----------------------------------------------------
	glm::vec4	avgColor(0);

	_averageColor.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexture);

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

	float	avgIllumination = (avgColor.x + avgColor.y + avgColor.z) / 3.0f;

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
