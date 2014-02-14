#include "LightRenderingSystem.hh"

#include <Components\MeshRenderer.hh>
#include <Context\IRenderContext.hh>
#include <OpenGL\VertexBuffer.hh>
#include <glm/gtc/matrix_transform.hpp>

LightRenderingSystem::LightRenderingSystem(AScene *scene) :
						System(scene),
						_lightFilter(scene),
						_meshRendererFilter(scene),
						_curFactor(1.0f),
						_targetFactor(1.0f),
						_useHDR(true)
{
}

LightRenderingSystem::~LightRenderingSystem()
{
	glDeleteFramebuffers(1, &_frameBuffer);
	glDeleteTextures(1, &_colorTexture);
	glDeleteTextures(1, &_depthTexture);
	glDeleteBuffers(1, &_lights);
	glDeleteBuffers(1, &_avgColors);
	delete	_vertexManager;
}

void LightRenderingSystem::initialize()
{
	_lightFilter.requireComponent<Component::PointLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();

	initQuad();
	initFrameBuffer();

	_averageColor.init("./ComputeShaders/AverageColorFirstPass.kernel");
	_modulateRender.init("./ComputeShaders/HighDynamicRange.kernel");

	// Gen light buffer
	glGenBuffers(1, &_lights);
	// Gen average color buffer
	glGenBuffers(1, &_avgColors);
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
	// ----------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer); // Bind the FrameBuffer to render to textures

	// Z PrePass
	// ----------------------------------------------------
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glColorMask(0, 0, 0, 0);

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
			if (s.getId() == earthId ||
				s.getId() == materialBasicId ||
				s.getId() == bumpId)
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, localLightBuffId);
			}
		}
		);
	}

	if (_useHDR)
		computeHdr();

	perFrame->setUniform("view", glm::mat4(1));
	perFrame->setUniform("projection", glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
	perFrame->flushChanges();

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Rebind the default FrameBuffer
	glDisable(GL_DEPTH_TEST);

	renderer->getShader("fboToScreen")->use();

	// Bind texture of the final render
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _colorTexture);

	_quad.draw();
}

void	LightRenderingSystem::initFrameBuffer()
{
	glm::ivec2 wDimensions = _scene->getInstance<IRenderContext>()->getScreenSize();

	// generate the deth texture
	glGenTextures(1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, wDimensions.x, wDimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// generate the color texture
	glGenTextures(1, &_colorTexture);
	glBindTexture(GL_TEXTURE_2D, _colorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, wDimensions.x, wDimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);

	// generate frame buffer
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

	// bind depth texture to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTexture, 0);
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

void		LightRenderingSystem::computeHdr()
{
	// ----------------------------------------------------
	// HDR Pass
	// ----------------------------------------------------
	// ----------------------------------------------------
	// Average colors:
	// ----------------------------------------------------
	_averageColor.use();

	GLint		colorBufferSizeLocation = glGetUniformLocation(_averageColor.getId(), "colorBufferSize");
	size_t		WORK_GROUP_SIZE = 16;
	glm::uvec2	fboSize = glm::uvec2(_scene->getInstance<IRenderContext>()->getScreenSize());
	glm::uvec2	groupNbr = glm::uvec2((fboSize.x + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE,
		(fboSize.y + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE);
	size_t		bufferSize = groupNbr.y * groupNbr.x;
	glm::vec4	*datas = new glm::vec4[bufferSize];
	glm::vec4	avgColor(0);

	// set the color buffer size
	glUniform2uiv(colorBufferSizeLocation, 1, glm::value_ptr(fboSize));
	// Allocate avg color buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _avgColors);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	// Bind color texture to sample
	glBindImageTexture(0, _colorTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
	// Bind average color buffer
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _avgColors);
	// Launch kernel
	glDispatchCompute(groupNbr.x, groupNbr.y, 1);
	// wait for the shader to finish writing on the buffer
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	// We get the buffer datas
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _avgColors);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, bufferSize * sizeof(glm::vec4), datas);
	// and we calcultae the average
	for (size_t i = 0; i < bufferSize; ++i)
	{
		avgColor += datas[i];
	}
	avgColor /= glm::vec4(bufferSize);
	delete[] datas;
	// ----------------------------------------------------
	// Modulate colors:
	// ----------------------------------------------------
	float	avgIllumination = (avgColor.x + avgColor.y + avgColor.z) / 3.0f;

	_targetFactor = glm::min(0.3f / avgIllumination, 1.0f);

	_modulateRender.use();

	colorBufferSizeLocation = glGetUniformLocation(_modulateRender.getId(), "colorBufferSize");
	GLint		avgIllumLocation = glGetUniformLocation(_modulateRender.getId(), "factor");

	if (_curFactor != _targetFactor)
	{
		if (_curFactor < _targetFactor)
		{
			_curFactor += 0.15f * _scene->getInstance<Timer>()->getElapsed();
			if (_curFactor > _targetFactor)
				_curFactor = _targetFactor;
		}
		else
		{
			_curFactor -= 0.15f * _scene->getInstance<Timer>()->getElapsed();
			if (_curFactor < _targetFactor)
				_curFactor = _targetFactor;
		}
	}

	// Set the uniforms
	glUniform2uiv(colorBufferSizeLocation, 1, glm::value_ptr(fboSize));
	glUniform1f(avgIllumLocation, _curFactor);

	// Bind color texture to modulate
	glBindImageTexture(0, _colorTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

	glDispatchCompute(groupNbr.x, groupNbr.y, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	// ----------------------------------------------------
}
