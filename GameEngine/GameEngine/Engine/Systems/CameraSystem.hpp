#ifndef   __CAMERA_SYSTEM_HPP__
# define  __CAMERA_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/MaterialComponent.h>
#include <Components/CameraComponent.hh>
#include <Entities/Entity.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Systems/MeshRenderSystem.h>

class CameraSystem : public System
{
public:
	CameraSystem(Engine &engine)
		: System(engine)
		, _renderDebugMethod(false)
	{}
	virtual ~CameraSystem(){}

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
		OpenGLTools::UniformBuffer *perFrameBuffer = _engine.getInstance<Renderer>().getUniform("PerFrame");

		for (auto e : _collection)
		{
			auto camera = e->getComponent<Component::CameraComponent>();
			auto skybox = camera->getSkybox();
			if (skybox.get())
			{
				OpenGLTools::Shader *s = _engine.getInstance<Renderer>().getShader(camera->getSkyboxShader());
				assert(s != NULL && "Skybox does not have a shader associated");

				_engine.getInstance<Renderer>().getUniform("cameraUniform")->setUniform("projection", camera->getProjection());

				glm::mat4 t = e->getGlobalTransform();
				t[3][0] = 0;
				t[3][1] = 0;
				t[3][2] = 0;
				t[3][3] = 1;

				_engine.getInstance<Renderer>().getUniform("cameraUniform")->setUniform("view", t);
				_engine.getInstance<Renderer>().getUniform("cameraUniform")->flushChanges();

//				_engine.getInstance<Renderer>().getFbo().bindDrawTargets(s->getTargets(), s->getTargetsNumber());

				s->use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getId());
				glDepthMask(0);
				skybox->draw();
				glDepthMask(1);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}

			t += time;
			// Set les uniforms du block PerFrame
			perFrameBuffer->setUniform("projection", camera->getProjection());
			perFrameBuffer->setUniform("view", e->getGlobalTransform());
			perFrameBuffer->setUniform("time", (float)t);
			perFrameBuffer->flushChanges();
			_engine.getInstance<SceneManager>().getCurrentScene()->getSystem<MeshRendererSystem>()->render(time);
		}
	}

	virtual void initialize()
	{
		require<Component::CameraComponent>();
	}
};

#endif    //__CAMERA_SYSTEM_HPP__