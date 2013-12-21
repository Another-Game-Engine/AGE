#ifndef    __CAMERA_COMPONENT_HPP__
# define   __CAMERA_COMPONENT_HPP__

#include "glm/glm.hpp"
#include "ResourceManager/CubeMap.hh"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.hh"
#include <Utils/SmartPointer.hh>

namespace Resources
{
	class CubeMap;
};

namespace Component
{
	class CameraComponent : public ComponentBase<CameraComponent>
	{
	private:
		CameraComponent(CameraComponent const &);
		CameraComponent	&operator=(CameraComponent const &);
		glm::mat4                        _projection;
		SmartPointer<Resources::CubeMap> _skybox;
		std::string                      _cubeMapShader;
	public:
		CameraComponent(Engine &engine, Handle &entity);
		virtual              ~CameraComponent(void);
		void init(){}
		virtual void reset(){}
		void                 attachSkybox(const std::string &name, const std::string &cubeMapShader);
		void                 dettachSkybox();
 		glm::mat4            &setProjection();
		glm::mat4            &getProjection();

	};
}

#endif     //__CAMERA_COMPONENT_HPP__