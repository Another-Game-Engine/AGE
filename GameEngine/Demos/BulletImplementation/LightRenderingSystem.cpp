#include "LightRenderingSystem.hh"

#include "PointLight.hh"
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
}

void LightRenderingSystem::initialize()
{
	_lightFilter.requireComponent<PointLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();

	initQuad();
	initFrameBuffer();

	// Gen light buffer
	glGenBuffers(1, &_lights);
}

void LightRenderingSystem::mainUpdate(double time)
{
	auto perFrame = _scene->getEngine().getInstance<Renderer>().getUniform("PerFrame");
	int lightNbr = _lightFilter.getCollection().size();

	perFrame->setUniform("lightNbr", lightNbr);
	// Create the contiguous light buffer
	if (_contiguousLights.size() != lightNbr)
	{
		_contiguousLights.clear();
		for (auto e : _lightFilter.getCollection())
		{
			_contiguousLights.push_back(*e->getComponent<PointLight>());
		}
	}
	else // Or just update it
	{
		size_t		i = 0;

		for (auto e : _lightFilter.getCollection())
		{
			_contiguousLights[i] = *e->getComponent<PointLight>();
			++i;
		}
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _lights);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightNbr * sizeof(PointLight), _contiguousLights.data(), GL_DYNAMIC_DRAW);
	// ----------------------------------------------------

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
	// Init the Quad:
	// ------------------------------------
	// x,y vertex positions
	float ss_quad_pos[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0, 1.0,
		1.0, 1.0,
		-1.0, 1.0,
		-1.0, -1.0
	};

	// per-vertex texture coordinates
	float ss_quad_st[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0
	};

	unsigned int indice[] = { 0, 1, 2, 3, 4, 5 };

	_quad.init(6, &indice[0]);
	_quad.addAttribute(OpenGLTools::Attribute(sizeof(float)* 2, 2, GL_FLOAT));
	_quad.addAttribute(OpenGLTools::Attribute(sizeof(float)* 2, 2, GL_FLOAT));
	_quad.setBuffer(0, reinterpret_cast<byte *>(&ss_quad_pos));
	_quad.setBuffer(1, reinterpret_cast<byte *>(&ss_quad_st));
	// ------------------------------------
}
