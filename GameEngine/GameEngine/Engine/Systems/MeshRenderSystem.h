#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/MaterialComponent.h>
#include <Entities/Entity.hh>
#include <Core/SceneManager.hh>

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(Engine &engine)
		: System(engine)
		, _renderDebugMethod(false)
	{}
	virtual ~MeshRendererSystem(){}

	void setRenderDebugMode(bool t)
	{
		_renderDebugMethod = t;
	}

	bool getRenderDebugMode() const
	{
		return _renderDebugMethod;
	}

protected:

	std::map<SmartPointer<Material>, std::list<Handle> > _sorted;
	bool _renderDebugMethod;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		static double t = 0;
		unsigned int textureOffset = 0;
		auto &renderer = _engine.getInstance<Renderer>();
		OpenGLTools::Framebuffer &fbo = renderer.getFbo();
		OpenGLTools::UniformBuffer *perFrameBuffer = _engine.getInstance<Renderer>().getUniform("PerFrame");

		auto currentScene = _engine.getInstance<SceneManager>().getCurrentScene();

		fbo.renderBegin();
		fbo.applyViewport();
		fbo.clearColor();
		fbo.clearZ();

		//render skybox before z-pass
		// TODO : !!!! TRANSFORM CAMERA INTO COMPONENT !!!
		_engine.getInstance<SceneManager>().getCurrentScene()->getCamera()->update();

		t += time;
		// Set les uniforms du block PerFrame
		perFrameBuffer->setUniform("projection", currentScene->getCamera()->getProjection());
		perFrameBuffer->setUniform("view", currentScene->getCamera()->getTransform());
		perFrameBuffer->setUniform("time", (float)t);
		perFrameBuffer->flushChanges();

		// temporary z-pass
		// to erase when depthOnly shader fixed

		//fbo.zPassBegin();

		OpenGLTools::UniformBuffer *perModelUniform = _engine.getInstance<Renderer>().getUniform("PerModel");

		//for (auto &mat : _sorted)
		//{
		//	for (auto &shaderName : mat.first->getShaders())
		//	{
		//		auto shader = renderer.getShader(shaderName);
		//		shader->use();
		//		for (auto &e : mat.second)
		//		{
		//			auto &mesh = e->getComponent<Component::MeshRenderer>();
		//			perModelUniform->setUniform("model", e->getGlobalTransform());
		//			perModelUniform->flushChanges();
		//			mesh->getMesh()->draw();
		//		}
		//	}
		//}

		//fbo.zPassEnd();

		// Render pass

		for (auto &mat : _sorted)
		{
			for (auto &shaderName : mat.first->getShaders())
			{
				auto shader = _engine.getInstance<Renderer>().getShader(shaderName);
				fbo.bindDrawTargets(shader->getTargets(), shader->getTargetsNumber());
				shader->use();
				// /!\ THAT'S VERY STRANGE WHEN I DON'T USE THAT THAT'S STILL WORKING !!
				//fbo.bind(shader);
				for (auto &e : mat.second)
				{
					auto &mesh = e->getComponent<Component::MeshRenderer>();
					perModelUniform->setUniform("model", e->getGlobalTransform());
					perModelUniform->flushChanges();
					mesh->bindTextures(shader);
					mesh->getMesh()->draw();
					//mesh->unbindTextures();
				}
				//fbo.unbind();
			}
		}

		fbo.bindDrawTargets(nullptr, 0);
		fbo.renderEnd();

		//fbo.renderBegin();
		//fbo.bindDrawTargets(renderer.getShader("brightnessFilter")->getTargets(), renderer.getShader("brightnessFilter")->getTargetsNumber());
		//fbo.renderRect(renderer.getShader("brightnessFilter"));
		//fbo.bindDrawTargets(renderer.getShader("blurY")->getTargets(), renderer.getShader("blurY")->getTargetsNumber());
		//fbo.renderRect(renderer.getShader("blurY"));
		//fbo.renderEnd();

		if (_renderDebugMethod)
			fbo.debugRendering(renderer.getShader("fboToScreen"));
		else
			fbo.renderRect(renderer.getShader("fboToScreen"));
	}

	virtual void initialize()
	{
		require<Component::MeshRenderer>();
		require<Component::MaterialComponent>();
		globalSub("MaterialComponentChanged", [&](Handle &e, SmartPointer<Material> oldMaterial, SmartPointer<Material> newMaterial)
		{
			if (_collection.find(e) == std::end(_collection) || oldMaterial == nullptr)
				return;
			if (_sorted.find(oldMaterial) != std::end(_sorted))
			{
				_sorted[oldMaterial].remove(e);
			}
			if (!newMaterial.get())
				return;
			_sorted[newMaterial].push_back(e);
		});
	}

	virtual void _componentAdded(Handle &e, unsigned int typeId)
	{
		if (_code.match(e->getCode()) && _collection.find(e) == std::end(_collection))
		{
			auto m = e->getComponent<Component::MaterialComponent>()->getMaterial();
			if (_sorted.find(m) == std::end(_sorted))
			{
				_sorted.insert(std::make_pair(m, std::list<Handle>()));
			}
			_collection.insert(e);
			_sorted[e->getComponent<Component::MaterialComponent>()->getMaterial()].push_back(e);
		}
	}

	virtual void _componentRemoved(Handle &e, unsigned int typeId)
	{
		if (!_code.match(e->getCode()) && _collection.find(e) != std::end(_collection))
		{
			_collection.erase(e);
			if (e->getComponent<Component::MaterialComponent>() != nullptr)
			{
				_sorted[e->getComponent<Component::MaterialComponent>()->getMaterial()].remove(e);
			}
		}
	}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__