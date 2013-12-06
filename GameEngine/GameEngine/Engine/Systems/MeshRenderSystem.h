#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/MaterialComponent.h>
#include <Entities/Entity.hh>

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

private:

	std::map<Material*, std::list<Entity*> > _sorted;
	bool _renderDebugMethod;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		static double t = 0;
		Engine	&e = *GameEngine::instance();
		unsigned int textureOffset;
		Renderer &renderer = e.renderer();
		OpenGLTools::Framebuffer &fbo = renderer.getFbo();
		OpenGLTools::UniformBuffer *perFrameBuffer = e.renderer().getUniform("PerFrame");

		t += time;
		// Set les uniforms du block PerFrame
		perFrameBuffer->setUniform("projection", e.getCurrentScene()->getCamera()->getProjection());
		perFrameBuffer->setUniform("view", e.getCurrentScene()->getCamera()->getTransform());
		perFrameBuffer->setUniform("time", (float)t);
		perFrameBuffer->flushChanges();

		fbo.renderBegin();
		fbo.applyViewport();
		fbo.clearColor();
		fbo.clearZ();

		//render skybox before z-pass

		GameEngine::instance()->getCurrentScene()->getCamera()->update();

		// temporary z-pass
		// to erase when depthOnly shader fixed

		fbo.zPassBegin();

		OpenGLTools::UniformBuffer *perModelUniform = GameEngine::instance()->renderer().getUniform("PerModel");

		for (auto &mat : _sorted)
		{
			for (auto &shaderName : mat.first->getShaders())
			{
				auto shader = renderer.getShader(shaderName);
				shader->use();
				for (auto &e : mat.second)
				{
					auto &mesh = e->getComponent<Component::MeshRenderer>();
					perModelUniform->setUniform("model", e->getGlobalTransform());
					perModelUniform->flushChanges();
					mesh->getMesh()->draw();
				}
			}
		}

		fbo.zPassEnd();

		// Render pass

		for (auto &mat : _sorted)
		{
			for (auto &shaderName : mat.first->getShaders())
			{
				auto shader = GameEngine::instance()->renderer().getShader(shaderName);
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

		fbo.renderBegin();
		fbo.bindDrawTargets(renderer.getShader("brightnessFilter")->getTargets(), renderer.getShader("brightnessFilter")->getTargetsNumber());
		fbo.renderRect(renderer.getShader("brightnessFilter"));
		fbo.bindDrawTargets(renderer.getShader("blurY")->getTargets(), renderer.getShader("blurY")->getTargetsNumber());
		fbo.renderRect(renderer.getShader("blurY"));
		fbo.renderEnd();

		if (_renderDebugMethod)
			fbo.debugRendering(renderer.getShader("fboToScreen"));
		else
			fbo.renderRect(renderer.getShader("fboToScreen"));
	}

	virtual void initialize()
	{
		require<Component::MeshRenderer>();
		require<Component::ComponentMaterial>();
		sub("MaterialComponentChanged", [&](Entity *e, Material *oldMaterial, Material *newMaterial)
		{
			if (_collection.find(e) == std::end(_collection) || oldMaterial == nullptr)
				return;
			if (_sorted.find(oldMaterial) != std::end(_sorted))
			{
				_sorted[oldMaterial].remove(e);
			}
			if (newMaterial == nullptr)
				return;
			_sorted[newMaterial].push_back(e);
		});
	}

	template <typename T>
	void _componentAdded(Entity *e)
	{
		if (_code.match(*e) && _collection.find(e) == std::end(_collection))
		{
			_collection.insert(e);
			_sorted[e->getComponent<Component::ComponentMaterial>()->getMaterial()] = e;
		}
	}

	template <typename T>
	void _componentRemoved(Entity *e)
	{
		if (!_code.match(*e) && _collection.find(e) != std::end(_collection))
		{
			_collection.erase(e);
			if (e->getComponent<Component::ComponentMaterial>() != nullptr)
			{
				_sorted[e->getComponent<Component::ComponentMaterial>()->getMaterial()].remove(e);
			}
		}
	}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__