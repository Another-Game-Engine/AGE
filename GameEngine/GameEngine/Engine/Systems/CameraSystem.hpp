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

///
/// TO MOVE TO UTILS
///

glm::vec3 posFromMat4(const glm::mat4 &mat)
{
	return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
}

glm::vec3 scaleFromMat4(const glm::mat4 &m)
{
	// Extract col vectors of the matrix
	glm::vec3 col1(m[0][0], m[0][1], m[0][2]);
	glm::vec3 col2(m[1][0], m[1][1], m[1][2]);
	glm::vec3 col3(m[2][0], m[2][1], m[2][2]);
	//Extract the scaling factors
	glm::vec3 scaling;
	scaling.x = glm::length(col1);
	scaling.y = glm::length(col2);
	scaling.z = glm::length(col3);
	return scaling;
}

glm::vec3 rotFromMat4(const glm::mat4 &m, bool toEuler)
{
	glm::vec3 scale = scaleFromMat4(m);
	glm::vec3 columns[3] = {
		glm::vec3(m[0][0], m[0][1], m[0][2]),
		glm::vec3(m[1][0], m[1][1], m[1][2]),
		glm::vec3(m[2][0], m[2][1], m[2][2])
	};
	if (scale.x)
		columns[0] /= scale.x;
	if (scale.y)
		columns[1] /= scale.y;
	if (scale.z)
		columns[2] /= scale.z;
	glm::mat3 mat(columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z);
	glm::quat quat(mat);
	glm::vec3 euler = glm::eulerAngles(quat);// ;
	if (toEuler)
		euler *= 3.14159f / 180.f;
	else
		euler *= -1;
	return euler;

}


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
			auto lookAt = e->getGlobalTransform();
			lookAt = glm::translate(lookAt, glm::vec3(0, 0, 1));

			auto cameraPosition = glm::lookAt(posFromMat4(e->getGlobalTransform()), posFromMat4(lookAt), glm::vec3(0, 1, 0));

			if (skybox.get())
			{
				OpenGLTools::Shader *s = _engine.getInstance<Renderer>().getShader(camera->getSkyboxShader());
				assert(s != NULL && "Skybox does not have a shader associated");

				_engine.getInstance<Renderer>().getUniform("cameraUniform")->setUniform("projection", camera->getProjection());

				glm::mat4 t = cameraPosition;
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
			perFrameBuffer->setUniform("view", cameraPosition);
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