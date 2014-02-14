#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/string.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include <OpenGL\Framebuffer.hh>

namespace Component
{
	struct CameraComponent : public ComponentBase<CameraComponent>
	{
		CameraComponent();
		virtual              ~CameraComponent(void);
		void init(){}
		virtual void reset(){}
		void                 attachSkybox(const std::string &name, const std::string &cubeMapShader);
		void                 dettachSkybox();
 		glm::mat4            &setProjection();
		glm::mat4            &getProjection();
		inline glm::mat4            &setLookAtTransform() { return lookAtTransform; }
		inline const glm::mat4      &getLookAtTransform() const { return lookAtTransform; }
		std::shared_ptr<CubeMapFile> getSkybox();
		const std::string &getSkyboxShader() const;

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new CameraComponent();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(projection), CEREAL_NVP(cubeMapShader), CEREAL_NVP(lookAtTransform));
		}

		// !Serialization
		////
		//////

		glm::uvec4						viewport;

		glm::mat4                       projection;
		std::shared_ptr<CubeMapFile>    skybox;
		std::string                     cubeMapShader;
		glm::mat4                       lookAtTransform;
		OpenGLTools::Framebuffer		frameBuffer;
	private:
		CameraComponent(CameraComponent const &);
		CameraComponent	&operator=(CameraComponent const &);
	};
}
