#pragma once

#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/CameraComponent.hpp>
#include <Core/Engine.hh>
#include <Context/SdlContext.hh>


class SimpleMeshRenderer : public System
{
public:
	SimpleMeshRenderer(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _meshs(std::move(scene))
		, _cameras(std::move(scene))
	{
		_name = "simple_mesh_renderer_system";
	}

	virtual ~SimpleMeshRenderer(){}
protected:

	EntityFilter _meshs;
	EntityFilter _cameras;
	std::shared_ptr<gl::Shader> _shader;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto scene = _scene.lock();

		auto renderer = _scene.lock()->getInstance<Renderer>();
		auto perFrame = renderer->getUniform("PerFrame");

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		_shader->use();

		for (auto &&c : _cameras.getCollection())
		{
			auto camera = scene->getComponent<Component::CameraComponent>(c);
			// Set les uniforms du block PerFrame
			perFrame->setUniform("projection", camera->projection);
			perFrame->setUniform("view", camera->lookAtTransform);
			//TODO pass the good time
			perFrame->setUniform("time", static_cast<float>(time));

			perFrame->flushChanges();

			for (auto &&mesh : _meshs.getCollection())
			{
				scene->getComponent<Component::MeshRenderer>(mesh)->renderRaw(renderer, scene->getTransform(mesh));
			}
		}
	}

	virtual bool initialize()
	{
		_meshs.requireComponent<Component::MeshRenderer>();
		_cameras.requireComponent<Component::CameraComponent>();
		_shader = _scene.lock()->getInstance<Renderer>()->addShader("basic",
			"../../Shaders/basic.vp",
			"../../Shaders/basic.fp");

		return true;
	}
};