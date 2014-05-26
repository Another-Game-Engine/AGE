#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/System.h"
#include <Components/Component.hh>
#include <Utils/OpenGL.hh>

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
			particleNumber = 1024 * 1024;
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

		for (std::uint64_t i = 0; i < emitter->particleNumber; ++i)
		{
			points[i].x = (float)((std::rand() % 1000) - 500.0f) / 100.0f;
			points[i].y = (float)((std::rand() % 1000) - 500.0f) / 100.0f;
			points[i].z = (float)((std::rand() % 1000) - 500.0f) / 100.0f;
			points[i].w = 1.0f;
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	// VEL
	{
		glGenBuffers(1, &emitter->velSSbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter->velSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, emitter->particleNumber * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec4 *points = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, emitter->particleNumber * sizeof(glm::vec4), bufMask);

		for (std::uint64_t i = 0; i < emitter->particleNumber; ++i)
		{
			points[i].x = (float)((std::rand() % 10) - 5.0f) / 10.0f;
			points[i].y = (float)((std::rand() % 10) - 5.0f) / 10.0f;
			points[i].z = (float)((std::rand() % 10) - 5.0f) / 10.0f;
			points[i].w = 1.0f;
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	// COL
	{
		glGenBuffers(1, &emitter->colSSbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter->colSSbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, emitter->particleNumber * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);

		GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glm::vec4 *points = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, emitter->particleNumber * sizeof(glm::vec4), bufMask);

		for (std::uint64_t i = 0; i < emitter->particleNumber; ++i)
		{
			points[i].x = (float)((std::rand() % 1000)) / 1000.0f;
			points[i].y = (float)((std::rand() % 1000)) / 1000.0f;
			points[i].z = (float)((std::rand() % 1000)) / 1000.0f;
			points[i].w = 1.0f;
		}
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

}

class ParticleSystem : public System
{
public:
	ParticleSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "particle_system";
	}
	virtual ~ParticleSystem(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto scene = _scene.lock();
		EntityFilter::Lock lock(_filter);

		//glUseProgram() // particleProgram;
		for (auto &&e : _filter.getCollection())
		{
			auto emitter = scene->getComponent<Component::ParticleEmitter>(e);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, emitter->posSSbo);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, emitter->velSSbo);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, emitter->colSSbo);
			glDispatchCompute(emitter->particleNumber / emitter->workGroupSize, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			//glUseProgram(); // my rendering program
			glBindBuffer(GL_ARRAY_BUFFER, emitter->posSSbo);
			glVertexPointer(4, GL_FLOAT, 0, (void*)0);
			glEnableClientState(GL_VERTEX_ARRAY);
			glDrawArrays(GL_POINTS, 0, emitter->particleNumber);
			glDisableClientState(GL_VERTEX_ARRAY);
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