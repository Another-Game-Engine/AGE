#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/string.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include <MediaFiles/AssetsManager.hpp>
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
		void save(Archive &ar) const
		{
			ar(CEREAL_NVP(projection), CEREAL_NVP(cubeMapShader), CEREAL_NVP(lookAtTransform));
			if (skybox != nullptr)
			{
				ar(skybox->path.getFullName());
			}
			else
				ar(std::string("NULL"));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(projection, cubeMapShader, lookAtTransform);
			std::string _skybox;
			ar(_skybox);
			if (_skybox != "NULL")
				skybox = _entity->getScene()->getInstance<AssetsManager>()->getFromFile<CubeMapFile>(File(_skybox));
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
