#ifndef   __CAMERA_SYSTEM_HPP__
# define  __CAMERA_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Utils/ScreenPosToWorldRay.hpp>
#include <Context/IRenderContext.hh>
#include <Utils/MatrixConversion.hpp>
#include <Text/FontManager.hh>

class CameraSystem : public System
{
public:
	CameraSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
		, _renderDebugMethod(false)
		, _totalTime(0)
	{
		_name = "camera_system";
	}

	virtual ~CameraSystem(){}

	void setRenderDebugMode(bool t)
	{
		_renderDebugMethod = t;
	}

	bool getRenderDebugMode() const
	{
		return _renderDebugMethod;
	}

	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to)
	{
		if (_filter.getCollection().size() == 0)
			return;
		auto scene = _scene.lock();
		auto mousePos = scene->getInstance<Input>()->getMousePosition();
		auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
		auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_filter.getCollection().begin()));
		screenPosToWorldRay(mousePos.x, mousePos.y, screenSize.x, screenSize.y, cameraCpt->lookAtTransform, cameraCpt->projection, from, to);
	}

	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to)
	{
		if (_filter.getCollection().size() == 0)
			return;
		auto scene = _scene.lock();
		auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
		auto centerPos = glm::vec2(screenSize) * glm::vec2(0.5f);
		auto cameraCpt = scene->getComponent<Component::CameraComponent>(*(_filter.getCollection().begin()));
		screenPosToWorldRay(
			static_cast<int>(centerPos.x),
			static_cast<int>(centerPos.y),
			static_cast<int>(screenSize.x),
			static_cast<int>(screenSize.y),
			cameraCpt->lookAtTransform,
			cameraCpt->projection,
			from,
			to);
	}

	// Returns the number of seconds since the component creation
	double		getLifeTime() const
	{
		return (_totalTime);
	}

protected:
	EntityFilter _filter;

	bool _renderDebugMethod;
	double	_totalTime;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{
	}

	virtual void mainUpdate(double time)
	{
		unsigned int textureOffset = 0;
		auto scene = _scene.lock();
		auto &renderer = scene->getInstance<Renderer>();
		std::shared_ptr<OpenGLTools::UniformBuffer> perFrameBuffer = scene->getInstance<Renderer>()->getUniform("PerFrame");

		for (auto e : _filter.getCollection())
		{
			auto camera = scene->getComponent<Component::CameraComponent>(e);
			auto skybox = camera->getSkybox();

			auto cameraPosition = camera->lookAtTransform;
			OpenGLTools::Framebuffer &camFbo = scene->getComponent<Component::CameraComponent>(e)->frameBuffer;

			if (skybox != nullptr && camFbo.isInit() == true)
			{
				std::shared_ptr<OpenGLTools::Shader> s = scene->getInstance<Renderer>()->getShader(camera->getSkyboxShader());
				assert(s != nullptr && "Skybox does not have a shader associated");

				scene->getInstance<Renderer>()->getUniform("cameraUniform")->setUniform("projection", camera->projection);

				glm::mat4 t = cameraPosition;
				t[3][0] = 0;
				t[3][1] = 0;
				t[3][2] = 0;
				t[3][3] = 1;

				scene->getInstance<Renderer>()->getUniform("cameraUniform")->setUniform("view", t);
				scene->getInstance<Renderer>()->getUniform("cameraUniform")->flushChanges();

				camFbo.bind();
				glViewport(0, 0, camFbo.getSize().x, camFbo.getSize().y);

				glEnable(GL_DEPTH_TEST);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				glDepthMask(GL_TRUE);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClearDepth(1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				s->use();

				glDepthMask(GL_FALSE);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getId());
				skybox->draw();
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				glDepthMask(GL_TRUE);
			}
		}
		_totalTime += time;
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::CameraComponent>();
		return true;
	}
};

#endif    //__CAMERA_SYSTEM_HPP__