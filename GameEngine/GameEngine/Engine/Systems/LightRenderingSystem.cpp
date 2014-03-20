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
						_shadowDimensions(2048, 2048)
{
}

LightRenderingSystem::~LightRenderingSystem()
{
	glDeleteTextures(1, &_spotShadowTextures);
	glDeleteFramebuffers(1, &_shadowsFbo);
}

void LightRenderingSystem::initialize()
{
	_pointLightFilter.requireComponent<Component::PointLight>();
	_spotLightFilter.requireComponent<Component::SpotLight>();
	_meshRendererFilter.requireComponent<Component::MeshRenderer>();
	_cameraFilter.requireComponent<Component::CameraComponent>();

	auto materialBasic = _scene.lock()->getInstance<Renderer>()->getShader("MaterialBasic");

	// And lights uniform buffer
	_scene.lock()->getInstance<Renderer>()->addUniform("pointLightBuff")
		->init(materialBasic, "pointLightBuff", POINT_LIGHT_BUFF_SIZE);
	_scene.lock()->getInstance<Renderer>()->addUniform("spotLightBuff")
		->init(materialBasic, "spotLightBuff", SPOT_LIGHT_BUFF_SIZE);

	_scene.lock()->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "pointLightBuff", "pointLightBuff");
	_scene.lock()->getInstance<Renderer>()->bindShaderToUniform("MaterialBasic", "spotLightBuff", "spotLightBuff");

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
	glEnable(GL_CULL_FACE);

	auto renderer = _scene.lock()->getInstance<Renderer>();
	auto perFrame = renderer->getUniform("PerFrame");

	// set all the lights in the GPU uniform buffers
	updateLights(perFrame);

	// -------------------- Render -------------------- \\
	// For each camera
	for (auto c : _cameraFilter.getCollection())
	{
		auto camera = c->getComponent<Component::CameraComponent>();
		// Set les uniforms du block PerFrame
		perFrame->setUniform("projection", camera->projection);
		perFrame->setUniform("view", camera->lookAtTransform);
		perFrame->setUniform("time", static_cast<float>(_scene.lock()->getSystem<CameraSystem>()->getLifeTime()));

		perFrame->flushChanges();

		if (camera->frameBuffer.isInit() == false)
			c->getComponent<Component::CameraComponent>()->initFrameBuffer(_scene.lock()->getInstance<IRenderContext>()->getScreenSize(), 1);

		glViewport(0, 0, camera->frameBuffer.getSize().x, camera->frameBuffer.getSize().y);

		// First Pass
		computeCameraRender(camera->frameBuffer, perFrame);
	}
	glDisable(GL_CULL_FACE);
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