#pragma once

#include <Entities/EntityTypedef.hpp>
#include <Utils/Dependency.hpp>
#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Geometry/Material.hpp>
#include <Core/PrepareKey.hpp>
#include <Utils/CommandQueueHolder.hpp>
#include <Utils/ThreadQueue.hpp>
#include <Skinning/AnimationInstance.hpp>

#define LOOSE_OCTREE_CULLING
//#define OCTREE_CULLING

#if defined LOOSE_OCTREE_CULLING || defined OCTREE_CULLING
#define ACTIVATE_OCTREE_CULLING
#endif

class AScene;

namespace AGE
{
	typedef std::uint64_t DRAWABLE_ID;
	typedef std::uint64_t USER_OBJECT_ID;

	struct Drawable;
	struct Mesh;
	struct PointLight;
	struct Camera;
	struct DrawableCollection;
	struct SubMeshInstance;

	class LooseOctreeNode;
	class OctreeNode;

	class PrepareRenderThread : public ThreadQueue, public Dependency<PrepareRenderThread>
	{
	public:
		PrepareRenderThread();
		virtual ~PrepareRenderThread(void);
		void setScene(std::weak_ptr<AScene> _scene) { scene = _scene; }

		PrepareKey addMesh();
		PrepareKey addCamera();
		PrepareKey addPointLight();

		PrepareRenderThread &setPointLight(glm::vec3 const &, glm::vec3 const &, const PrepareKey &id);
		PrepareRenderThread &removeElement(const PrepareKey &key);
		PrepareRenderThread &setPosition(const glm::vec3 &v, const PrepareKey &key);
		PrepareRenderThread &setOrientation(const glm::quat &v, const PrepareKey &key);
		PrepareRenderThread &setScale(const glm::vec3 &v, const PrepareKey &key);
		PrepareRenderThread &setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		PrepareRenderThread &setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		PrepareRenderThread &setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		PrepareRenderThread &updateGeometry(
			const PrepareKey &id
			, const Vector<SubMeshInstance> &meshs
			, const Vector<MaterialInstance> &materials
			, const gl::Key<AGE::AnimationInstance> &animation);
		PrepareRenderThread &setCameraInfos(const PrepareKey &id, const glm::mat4 &projection);

	private:
#if defined LOOSE_OCTREE_CULLING
		LooseOctreeNode *_octree;
#elif defined OCTREE_CULLING
		OctreeNode *_octree;
#endif

		std::weak_ptr<AScene> scene;

		Vector<Mesh> _meshs;
		Vector<Drawable> _drawables;
		Vector<Camera> _cameras;
		Vector<PointLight> _pointLights;
		Queue<PrepareKey::OctreeObjectId> _freeDrawables;
		Queue<PrepareKey::OctreeObjectId> _freeCameras;
		Queue<PrepareKey::OctreeObjectId> _freeMeshs;
		Queue<PrepareKey::OctreeObjectId> _freePointLights;
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
	};

	namespace Threads
	{
		typedef PrepareRenderThread Prepare;
	}
}