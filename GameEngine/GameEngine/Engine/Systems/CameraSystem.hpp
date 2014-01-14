#ifndef   __CAMERA_SYSTEM_HPP__
# define  __CAMERA_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/MaterialComponent.h>
#include <Components/CameraComponent.hh>
#include <Entities/EntityData.hh>
#include <Core/SceneManager.hh>
#include <Core/Renderer.hh>
#include <Systems/MeshRenderSystem.h>
// to erase
#include <Components/TrackBallComponent.hpp>

class CameraSystem : public System
{
public:
	CameraSystem(AScene *scene)
		: System(scene)
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
		static double totalTime = 0;
		unsigned int textureOffset = 0;
		auto &renderer = _scene->getEngine().getInstance<Renderer>();
		OpenGLTools::UniformBuffer *perFrameBuffer = _scene->getEngine().getInstance<Renderer>().getUniform("PerFrame");

		for (auto e : _collection)
		{
			auto camera = e->getComponent<Component::CameraComponent>();
			auto skybox = camera->getSkybox();
			auto lookAt = e->getGlobalTransform();
			lookAt = glm::translate(lookAt, glm::vec3(0, 0, 1));

			auto cameraPosition = camera->getLookAtTransform();

			if (skybox.get())
			{
				OpenGLTools::Shader *s = _scene->getEngine().getInstance<Renderer>().getShader(camera->getSkyboxShader());
				assert(s != NULL && "Skybox does not have a shader associated");

				_scene->getEngine().getInstance<Renderer>().getUniform("cameraUniform")->setUniform("projection", camera->getProjection());

				glm::mat4 t = cameraPosition;
				t[3][0] = 0;
				t[3][1] = 0;
				t[3][2] = 0;
				t[3][3] = 1;

				_scene->getEngine().getInstance<Renderer>().getUniform("cameraUniform")->setUniform("view", t);
				_scene->getEngine().getInstance<Renderer>().getUniform("cameraUniform")->flushChanges();

//				_engine.getInstance<Renderer>().getFbo().bindDrawTargets(s->getTargets(), s->getTargetsNumber());

				s->use();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getId());
				glDepthMask(0);
				skybox->draw();
				glDepthMask(1);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}

			totalTime += time;



			// Set les uniforms du block PerFrame
			perFrameBuffer->setUniform("projection", camera->getProjection());
			perFrameBuffer->setUniform("view", cameraPosition);
			perFrameBuffer->setUniform("time", (float)totalTime);
			perFrameBuffer->flushChanges();
			_scene->getSystem<MeshRendererSystem>()->render(time);
		}
	}

	virtual void initialize()
	{
		require<Component::CameraComponent>();
	}
};

#endif    //__CAMERA_SYSTEM_HPP__