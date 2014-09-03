#pragma once

#include <glm/glm.hpp>
#include <Components/Component.hh>
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/string.hpp>
#include "Behaviors/Camera.hpp"

namespace Component
{
	struct CameraComponent : public ComponentBase<CameraComponent>, public AGE::ComponentBehavior::Camera
	{
		CameraComponent();
		virtual ~CameraComponent(void);
		void init(AScene *scene)
		{
			::AGE::ComponentBehavior::Camera::init(scene, this->entityId);
		}

		virtual void reset(AScene *scene)
		{
			::AGE::ComponentBehavior::Camera::reset(scene, this->entityId);
		}

		void                 dettachSkybox();
		const std::string &getSkyboxShader() const;

		//////
		////
		// Serialization

		template <typename Archive>
		void save(Archive &ar) const
		{
		}

		template <typename Archive>
		void load(Archive &ar)
		{
		}

		// !Serialization
		////
		//////

		glm::uvec4	viewport;
		std::string cubeMapShader;
		bool blitOnScreen;

		// Camera fbo infos
		glm::uvec2	fboSize;
		uint32_t	sampleNbr;

		CameraComponent(CameraComponent const &o)
			: AGE::ComponentBehavior::Camera(o)
		{
			viewport = o.viewport;
			cubeMapShader = o.cubeMapShader;
			//@CESAR TODO TODO COPY FRAMEBUFFER
			// @CESAR IMPORTANT FBO ARE COPYED ! THIS HAVE TO BE TEMPORARY !!!!
			blitOnScreen = o.blitOnScreen;
			fboSize = o.fboSize;
			sampleNbr = o.sampleNbr;
		}

		CameraComponent	&operator=(CameraComponent const &o)
		{
			viewport = o.viewport;
			cubeMapShader = o.cubeMapShader;
			//@CESAR TODO TODO COPY FRAMEBUFFER
			// @CESAR IMPORTANT FBO ARE COPYED ! THIS HAVE TO BE TEMPORARY !!!!
			blitOnScreen = o.blitOnScreen;
			fboSize = o.fboSize;
			sampleNbr = o.sampleNbr;
			AGE::ComponentBehavior::Camera::operator=(o);
			return *this;
		}
	private:
	};
}
