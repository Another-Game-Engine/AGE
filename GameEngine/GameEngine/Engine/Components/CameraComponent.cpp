#include <Components/CameraComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Context/SDL/SdlContext.hh>
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

	void CameraComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const CameraComponent*>(model);
		_data = o->_data;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_data.projection = projection;
		AGE::GetPrepareThread()->setCameraInfos(_data.projection, _key, _data.pipeline);
	}

	void CameraComponent::setPipeline(RenderType pipeline)
	{
		_data.pipeline = pipeline;
		AGE::GetPrepareThread()->setCameraInfos(_data.projection, _key, _data.pipeline);
	}

	void CameraComponent::setTexture(std::shared_ptr<Texture3D> const &texture)
	{
		_data.texture = texture;
		AGE::GetPrepareThread()->setCameraInfos(_data.projection, _key, _data.pipeline);
	}

	void CameraComponent::init()
	{
		auto scene = entity.getScene();
		_key = AGE::GetPrepareThread()->addCamera();
		entity.getLink().registerOctreeObject(_key);
		auto screenSize = entity.getScene()->getInstance<IRenderContext>()->getScreenSize();
		setProjection(glm::perspective(60.0f, (float)screenSize.x / (float)screenSize.y, 0.1f, 2000.0f));
	}

	void CameraComponent::reset()
	{
		if (!_key.invalid())
		{
			entity.getLink().unregisterOctreeObject(_key);
		}
		_data = CameraData();
	}

	void CameraComponent::postUnserialization()
	{
		init();
		AGE::GetPrepareThread()->setCameraInfos(_data.projection, _key, _data.pipeline);
	}

#ifdef EDITOR_ENABLED
	void CameraComponent::editorCreate()
	{}

	void CameraComponent::editorDelete()
	{}

	bool CameraComponent::editorUpdate()
	{
		return false;
	}
#endif

};

