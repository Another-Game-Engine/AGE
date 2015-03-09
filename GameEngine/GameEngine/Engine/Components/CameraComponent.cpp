#include <Components/CameraComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Context/SdlContext.hh>
#include <Core/AScene.hh>
#include <imgui/imgui.h>

namespace AGE
{
	CameraComponent::CameraComponent()
		: ComponentBase()
	{
	}

	CameraComponent::~CameraComponent(void)
	{
	}

	CameraComponent::CameraComponent(CameraComponent const &o)
		: _scene(o._scene)
		, _projection(1)
		, _key(o._key)
	{
	}

	CameraComponent	&CameraComponent::operator=(CameraComponent const &o)
	{
		_scene = o._scene;
		_key = o._key;
		_projection = o._projection;
		return *this;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_projection = projection;

		AGE::GetPrepareThread()->setCameraInfos(_projection, _key, _pipelines);
	}

	void CameraComponent::addPipeline(RenderType pipeline)
	{
		auto haveIt = havePipeline(pipeline);
		if (haveIt == false)
		{
			_pipelines.insert(pipeline);
			AGE::GetPrepareThread()->setCameraInfos(_projection, _key, _pipelines);
		}
	}

	void CameraComponent::removePipeline(RenderType pipeline)
	{
		auto it = _pipelines.find(pipeline);
		if (it  != std::end(_pipelines))
		{
			_pipelines.erase(it);
			AGE::GetPrepareThread()->setCameraInfos(_projection, _key, _pipelines);
		}
	}

	bool CameraComponent::havePipeline(RenderType pipeline) const
	{
		auto it = _pipelines.find(pipeline);
		return (it != std::end(_pipelines));
	}


	const glm::mat4 &CameraComponent::getProjection() const
	{
		return _projection;
	}

	void CameraComponent::init(AScene *scene)
	{
		_scene = scene;
		_key = AGE::GetPrepareThread()->addCamera();
		entity.getLink().registerOctreeObject(_key);
		auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
		setProjection(glm::perspective(60.0f, (float)screenSize.x / (float)screenSize.y, 0.1f, 2000.0f));
	}

	void CameraComponent::reset(AScene *scene)
	{
		if (_key.invalid())
			entity.getLink().unregisterOctreeObject(_key);
		_projection = glm::mat4(1);
	}

	void CameraComponent::postUnserialization(AScene *scene)
	{
		init(scene);
		setProjection(_projection);
	}

#ifdef EDITOR_ENABLED
	void CameraComponent::editorCreate(AScene *scene)
	{}

	void CameraComponent::editorDelete(AScene *scene)
	{}

	void CameraComponent::editorUpdate(AScene *scene)
	{

	}
#endif

};

