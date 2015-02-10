#include <Components/CameraComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Context/SdlContext.hh>
#include <Core/AScene.hh>

namespace AGE
{
	namespace Component
	{
		CameraComponent::CameraComponent()
			: ComponentBase<CameraComponent>()
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

			AGE::GetPrepareThread()->setCameraProjection(_projection, _key);
		}

		const glm::mat4 &CameraComponent::getProjection() const
		{
			return _projection;
		}

		void CameraComponent::init(AScene *scene)
		{
			_scene = scene;
			_key = AGE::GetPrepareThread()->addCamera();
			scene->getLink(entity)->registerOctreeObject(_key);
			auto screenSize = scene->getInstance<IRenderContext>()->getScreenSize();
			setProjection(glm::perspective(60.0f, (float)screenSize.x / (float)screenSize.y, 0.1f, 2000.0f));
		}

		void CameraComponent::reset(AScene *scene)
		{
			assert(!_key.invalid());
			scene->getLink(entity)->unregisterOctreeObject(_key);
			_projection = glm::mat4(1);
		}

		void CameraComponent::postUnserialization(AScene *scene)
		{
			init(scene);
			setProjection(_projection);
		}

	};
}
