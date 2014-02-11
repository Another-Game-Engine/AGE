#include "LightRenderingSystem.hh"

#include <Components\MeshRenderer.hh>
#include <Context\IRenderContext.hh>
#include <OpenGL\VertexBuffer.hh>

LightRenderingSystem::LightRenderingSystem(AScene *scene) :
				System(scene),
				_lightFilter(scene),
				_meshRendererFilter(scene)
{
}

LightRenderingSystem::~LightRenderingSystem()
{
	delete	_vertexManager;
}

void LightRenderingSystem::initialize()
{
	_lightFilter.requireComponent<Component::PointLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();

	initQuad();
	initFrameBuffer();

	// Gen light buffer
	glGenBuffers(1, &_lights);
}

void LightRenderingSystem::mainUpdate(double time)
{
	Renderer &renderer = _scene->getEngine().getInstance<Renderer>();
	auto perFrame = renderer.getUniform("PerFrame");
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
			_contiguousLights.back().position = e->getGlobalTransform()[3];
		}
	}
	else // Or just update it
	{
		size_t		i = 0;

		for (auto e : _lightFilter.getCollection())
		{
			_contiguousLights[i] = e->getComponent<Component::PointLight>()->lightData;
			_contiguousLights[i].position = e->getGlobalTransform()[3];
			++i;
		}
	}

	size_t	toto = sizeof(Component::PointLight);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _lights);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightNbr * sizeof(ContiguousLight), _contiguousLights.data(), GL_DYNAMIC_DRAW);
	// ----------------------------------------------------
//	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer); // Bind the FrameBuffer to render to textures
//
//	// Z PrePass
//	// ----------------------------------------------------
//	glDrawBuffer(GL_COLOR_ATTACHMENT0);
//	glEnable(GL_DEPTH_TEST);
//	glClearDepth(1.0f);
//	glDepthFunc(GL_LESS);
//	glDepthMask(GL_TRUE);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	glColorMask(0, 0, 0, 0);
//
//	renderer.getShader("depthOnly")->use();
//
//	for (auto e : _meshRendererFilter.getCollection())
//	{
//		e->getComponent<Component::MeshRenderer>()->renderRaw();
//	}
//	glFinish();
//	// ----------------------------------------------------
//	// Final Lightning pass
//	// ----------------------------------------------------
//	glDepthFunc(GL_LEQUAL);
//	glDepthMask(GL_FALSE);
//	glColorMask(1, 1, 1, 1);
//
//	GLuint		localLightBuffId = _lights;
//
//	for (auto e : _meshRendererFilter.getCollection())
//	{
//		e->getComponent<Component::MeshRenderer>()->render(
//		[&renderer, localLightBuffId](OpenGLTools::Shader &s)
//		{
//			if (s.getId() == renderer.getShader("MaterialBasic")->getId())
//			{
//				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, localLightBuffId);
//			}
//		}
//		);
//	}
//	glFinish();
//	// ----------------------------------------------------
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Rebind the default FrameBuffer
//	glDisable(GL_DEPTH_TEST);
//
//	renderer.getShader("fboToScreen")->use();
//
//	// Bind texture of the final render
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, _colorTexture);
//
//	_quad.draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Rebind the default FrameBuffer

	GLuint		localLightBuffId = _lights;

	for (auto e : _meshRendererFilter.getCollection())
	{
		e->getComponent<Component::MeshRenderer>()->render(
		[&renderer, localLightBuffId](OpenGLTools::Shader &s)
		{
			if (s.getId() == renderer.getShader("MaterialBasic")->getId())
			{
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, localLightBuffId);
			}
		}
		);
	}
}

void	LightRenderingSystem::initFrameBuffer()
{
	glm::ivec2 wDimensions = _scene->getEngine().getInstance<IRenderContext>().getScreenSize();

	// generate the deth texture
	glGenTextures(1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, wDimensions.x, wDimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	// generate the color texture
	glGenTextures(1, &_colorTexture);
	glBindTexture(GL_TEXTURE_2D, _colorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, wDimensions.x, wDimensions.y, 0, GL_RGBA, GL_FLOAT, NULL);

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
