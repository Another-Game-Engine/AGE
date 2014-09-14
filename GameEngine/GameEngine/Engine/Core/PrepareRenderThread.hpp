#pragma once

#include <glm/fwd.hpp>
#include <Entities/EntityTypedef.hpp>
#include <Utils/Dependency.hpp>
#include <Utils/BoundingInfos.hpp>
#include <Entities/Entity.hh>
#include <Render/Key.hh>
#include <Render/Data.hh>
#include <stack>
#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Core/Drawable.hh>
#include <Core/PrepareKey.hpp>
#include <Utils/CommandQueueHolder.hpp>
#include <Utils/ThreadQueue.hpp>

class AScene;

namespace AGE
{
	class PrepareRenderThread : public ThreadQueue, public Dependency<PrepareRenderThread>
	{
	public:
		PrepareRenderThread();
		virtual ~PrepareRenderThread(void);
		void setScene(std::weak_ptr<AScene> _scene) { scene = _scene; }

		PrepareKey addMesh();
		PrepareKey addCamera();
		PrepareKey addPointLight();

		PrepareRenderThread &setPointLight(float, float, glm::vec3 const &, glm::vec4 const &, const PrepareKey &id);
		PrepareRenderThread &removeElement(const PrepareKey &key);
		PrepareRenderThread &setPosition(const glm::vec3 &v, const PrepareKey &key);
		PrepareRenderThread &setOrientation(const glm::quat &v, const PrepareKey &key);
		PrepareRenderThread &setScale(const glm::vec3 &v, const PrepareKey &key);
		PrepareRenderThread &setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		PrepareRenderThread &setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		PrepareRenderThread &setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		PrepareRenderThread &updateGeometry(const PrepareKey &id, const AGE::Vector<AGE::SubMeshInstance> &meshs, const AGE::Vector<AGE::MaterialInstance> &materials);
		PrepareRenderThread &setCameraInfos(const PrepareKey &id, const glm::mat4 &projection);

	private:
		std::weak_ptr<AScene> scene;

		AGE::Vector<Mesh> _meshs;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeMeshs;
		AGE::Vector<Drawable> _drawables;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeDrawables;
		AGE::Vector<Camera> _cameras;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeCameras;
		AGE::Vector<PointLight> _pointLights;
		AGE::Queue<PrepareKey::OctreeObjectId> _freePointLights;
		std::size_t _MeshCounter = 0;
		std::size_t _cameraCounter = 0;
		std::size_t _pointLightCounter = 0;

		AGE::Vector<DrawableCollection> _octreeDrawList;

		virtual bool _init();
		virtual bool _update();
		virtual bool _initInNewThread();
		virtual bool _release();
		virtual bool _releaseInNewThread();
		DRAWABLE_ID addDrawable(USER_OBJECT_ID uid);
		void removeDrawableObject(DRAWABLE_ID id);
		void _run();

		bool _isRunning;
	};

	namespace Threads
	{
		typedef PrepareRenderThread Prepare;
	}
}