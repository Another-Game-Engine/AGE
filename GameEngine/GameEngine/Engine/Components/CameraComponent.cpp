//# define GLM_FORCE_RADIANS

#include <Components/CameraComponent.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/matrix_inverse.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>

namespace Component
{
	CameraComponent::CameraComponent()
		: ComponentBase<CameraComponent>(),
		viewport(0),
		fboSize(800, 600),
		sampleNbr(1),
		blitOnScreen(true)
	{
	}

	CameraComponent::~CameraComponent(void)
	{}

	CameraComponent::CameraComponent(CameraComponent const &o)
		: AGE::OctreeElement(o)
	{
			viewport = o.viewport;
			cubeMapShader = o.cubeMapShader;
			//@CESAR TODO TODO COPY FRAMEBUFFER
			// @CESAR IMPORTANT FBO ARE COPYED ! THIS HAVE TO BE TEMPORARY !!!!
			blitOnScreen = o.blitOnScreen;
			fboSize = o.fboSize;
			sampleNbr = o.sampleNbr;
		}

	CameraComponent	&CameraComponent::operator=(CameraComponent const &o)
	{
		viewport = o.viewport;
		cubeMapShader = o.cubeMapShader;
		//@CESAR TODO TODO COPY FRAMEBUFFER
		// @CESAR IMPORTANT FBO ARE COPYED ! THIS HAVE TO BE TEMPORARY !!!!
		blitOnScreen = o.blitOnScreen;
		fboSize = o.fboSize;
		sampleNbr = o.sampleNbr;
		AGE::OctreeElement::operator=(o);
		return *this;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_projection = projection;
		_scene->getInstance<AGE::Octree>()->setCameraInfos(_OTKey, _projection);
	}

	const glm::mat4 &CameraComponent::getProjection() const
	{
		return _projection;
	}

	const std::string &CameraComponent::getSkyboxShader() const
	{
		return cubeMapShader;
	}

	void CameraComponent::init(AScene *scene)
	{
		_scene = scene;
		_projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
		initOctree(scene, this->entityId);
	}

	void CameraComponent::reset(AScene *scene)
	{
		resetOctree(scene, this->entityId);
	}

	AGE::OctreeElement &CameraComponent::updateOctree()
	{
		return (*this);
	}

	AGE::OctreeElement &CameraComponent::initOctree(::AScene *scene, ENTITY_ID entityId)
	{
		_OTKey = scene->getInstance<AGE::Octree>()->addCameraElement();
		scene->getLink(entityId)->registerOctreeObject(_OTKey);
		scene->getInstance<AGE::Octree>()->setCameraInfos(_OTKey, _projection);
		return (*this);
	}

	AGE::OctreeElement &CameraComponent::resetOctree(::AScene *scene, ENTITY_ID entityId)
	{
		scene->getLink(entityId)->unregisterOctreeObject(_OTKey);
		scene->getInstance<AGE::Octree>()->removeElement(_OTKey);
		return (*this);
	}

};

