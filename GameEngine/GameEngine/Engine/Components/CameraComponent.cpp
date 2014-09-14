#include <Components/CameraComponent.hpp>
#include <glm/fwd.hpp>

#include <Entities/Entity.hh>
#include <Core/AScene.hh>

namespace Component
{
	CameraComponent::CameraComponent()
		: ComponentBase<CameraComponent>(),
		AGE::PrepareElement(),
		viewport(0),
		fboSize(800, 600),
		sampleNbr(1),
		blitOnScreen(true)
	{
	}

	CameraComponent::~CameraComponent(void)
	{}

	CameraComponent::CameraComponent(CameraComponent const &o)
		: AGE::PrepareElement(o)
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
		AGE::PrepareElement::operator=(o);
		return *this;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_projection = projection;
		_scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_OTKey, _projection);
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

	AGE::PrepareElement &CameraComponent::initOctree(::AScene *scene, ENTITY_ID entityId)
	{
		_OTKey = scene->getInstance<AGE::Threads::Prepare>()->addCamera();
		scene->getLink(entityId)->registerOctreeObject(_OTKey);
		scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_OTKey, _projection);
		return (*this);
	}

	AGE::PrepareElement &CameraComponent::resetOctree(::AScene *scene, ENTITY_ID entityId)
	{
		scene->getLink(entityId)->unregisterOctreeObject(_OTKey);
		scene->getInstance<AGE::Threads::Prepare>()->removeElement(_OTKey);
		return (*this);
	}

};

