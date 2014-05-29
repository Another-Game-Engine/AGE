#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/System.h"
#include <Components/Component.hh>
#include <Utils/OpenGL.hh>
#include <OpenGL/AShader.hh>

namespace Component
{
	struct ParticleEmitter : public Component::ComponentBase<ParticleEmitter>
	{
		ParticleEmitter()
		{

		}

		virtual ~ParticleEmitter(void)
		{

		}

		ParticleEmitter(ParticleEmitter &&o)
			: ComponentBase<ParticleEmitter>(std::move(o))
		{
				particleNumber = std::move(o.particleNumber);
				workGroupSize = std::move(o.workGroupSize);
		}

		ParticleEmitter &operator=(ParticleEmitter &&o)
		{
			particleNumber = std::move(o.particleNumber);
			workGroupSize = std::move(o.workGroupSize);
			return *this;
		}

		void init()
		{
			particleNumber = 1024*1024;
			workGroupSize = 128;
		}

		virtual void reset()
		{
		}

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
			//ar(CEREAL_NVP(_t));
		}

		// !Serialization
		////
		//////
	public:
		std::uint64_t particleNumber;
		std::uint64_t workGroupSize;
		GLuint posSSbo;
		GLuint velSSbo;
		GLuint colSSbo;
	private:
		ParticleEmitter &operator=(ParticleEmitter const &o);
		ParticleEmitter(ParticleEmitter const &o);
	};
}

void ParticleEntityAdded(std::weak_ptr<AScene> scene, Entity &&e)
{
	auto emitter = scene.lock()->getComponent<Component::ParticleEmitter>(e);

	// POS
	{
		glGenBuffers(1, &emitter->posSSbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter->posSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, emitter->particleNumber * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec4 *points = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, emitter->particleNumber * sizeof(glm::vec4), bufMask);

		for (int i = 0; i < emitter->particleNumber; ++i)
		{
			points[i] = glm::vec4(((float)(rand() % 1000) - 500.0f), ((float)(rand() % 1000) - 500.0f), ((float)(rand() % 1000) - 500.0f),1);// (float)((std::rand() % 1000) - 500.0f);
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	// VEL
	{
		glGenBuffers(1, &emitter->velSSbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter->velSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, emitter->particleNumber * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec4 *points = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, emitter->particleNumber * sizeof(glm::vec4), bufMask);

		for (int i = 0; i < emitter->particleNumber; ++i)
		{
			points[i] = glm::vec4(((float)(std::rand() % 1000) - 500.0f) / 1000.0f, ((float)(std::rand() % 1000) - 500.0f) / 1000.0f, ((float)(std::rand() % 1000) - 500.0f) / 1000.0f, 1.0f);
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	// COL
	{
		glGenBuffers(1, &emitter->colSSbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter->colSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, emitter->particleNumber * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec4 *points = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, emitter->particleNumber * sizeof(glm::vec4), bufMask);

		for (int i = 0; i < emitter->particleNumber; ++i)
		{
			points[i] = glm::vec4((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, 1.0f);
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

}

class ParticleSystem : public System
{
public:
	ParticleSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
		, _computeShader(std::move(std::string("../../ComputeShaders/Particles.kernel")))
		, _renderShader(std::move(std::string("../../Shaders/Particles.vp")), std::move(std::string("../../Shaders/Particles.fp")), std::move(std::string("../../Shaders/Billboard.gp")))
	{
		_name = "particle_system";
	}
	virtual ~ParticleSystem(){}
private:
	EntityFilter _filter;
	OpenGLTools::Shader		_computeShader;
	OpenGLTools::Shader		_renderShader;

	glm::mat4 Projection = glm::perspective(65.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	glm::vec3 camPos = glm::vec3(0.3f, 0.3f, -1000);
	glm::mat4 View;
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !

	virtual void updateBegin(double time)
	{

	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto scene = _scene.lock();
		EntityFilter::Lock lock(_filter);

		camPos += glm::vec3(0, 0, 0.5f);

		View = glm::lookAt(
			camPos,
			camPos + glm::vec3(0,0,1),
			glm::vec3(0, 1, 0)
			);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto &&e : _filter.getCollection())
		{
			auto emitter = scene->getComponent<Component::ParticleEmitter>(e);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, emitter->posSSbo);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, emitter->velSSbo);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, emitter->colSSbo);

			_computeShader.use();

			glDispatchCompute(emitter->particleNumber / emitter->workGroupSize, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

			_renderShader.use();

			auto m = glGetUniformLocation(_renderShader.getId(), "Model");
			auto v = glGetUniformLocation(_renderShader.getId(), "View");
			auto p = glGetUniformLocation(_renderShader.getId(), "Projection");
			auto cp = glGetUniformLocation(_renderShader.getId(), "CameraPos");

			glUniformMatrix4fv(m, 1, GL_FALSE, &Model[0][0]);
			glUniformMatrix4fv(v, 1, GL_FALSE, &View[0][0]);
			glUniformMatrix4fv(p, 1, GL_FALSE, &Projection[0][0]);
			glUniform3fv(cp, 1, glm::value_ptr(camPos));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, emitter->posSSbo);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, emitter->colSSbo);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_POINTS, 0, emitter->particleNumber);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::ParticleEmitter>();
		_filter.entityAdded = ParticleEntityAdded;
		return true;
	}
};