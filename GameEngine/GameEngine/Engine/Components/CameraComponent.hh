#ifndef    __CAMERA_COMPONENT_HPP__
# define   __CAMERA_COMPONENT_HPP__

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"
#include <Utils/SmartPointer.hh>
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/string.hpp>

namespace Resources
{
	class CubeMap;
};

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
		SmartPointer<Resources::CubeMap> getSkybox();
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

		glm::mat4                        projection;
		SmartPointer<Resources::CubeMap> skybox;
		std::string                      cubeMapShader;
		glm::mat4                        lookAtTransform;
	private:
		CameraComponent(CameraComponent const &);
		CameraComponent	&operator=(CameraComponent const &);
	};
}

#endif     //__CAMERA_COMPONENT_HPP__