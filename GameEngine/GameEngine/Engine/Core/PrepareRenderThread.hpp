//#pragma once
//
//#include <Entities/EntityTypedef.hpp>
//#include <Utils/Dependency.hpp>
//#include <Utils/Containers/Queue.hpp>
//#include <Utils/Containers/Vector.hpp>
//#include <Geometry/Material.hpp>
//#include <Core/PrepareKey.hpp>
//#include <Skinning/AnimationInstance.hpp>
//
//class AScene;
//
//namespace AGE
//{
//	typedef std::uint64_t DRAWABLE_ID;
//	typedef std::uint64_t USER_OBJECT_ID;
//
//	struct Drawable;
//	struct Mesh;
//	struct PointLight;
//	struct Camera;
//	struct DrawableCollection;
//	struct SubMeshInstance;
//
//	class OctreeNode;
//
//	class OldPrepareRenderThread : public ThreadQueue, public Dependency<OldPrepareRenderThread>
//	{
//	public:
//		OldPrepareRenderThread();
//		virtual ~OldPrepareRenderThread(void);
//		void setScene(std::weak_ptr<AScene> _scene) { scene = _scene; }
//
//		PrepareKey addMesh();
//		PrepareKey addCamera();
//		PrepareKey addPointLight();
//
//		OldPrepareRenderThread &setPointLight(glm::vec3 const &, glm::vec3 const &, const PrepareKey &id);
//		OldPrepareRenderThread &removeElement(const PrepareKey &key);
//		OldPrepareRenderThread &setPosition(const glm::vec3 &v, const PrepareKey &key);
//		OldPrepareRenderThread &setOrientation(const glm::quat &v, const PrepareKey &key);
//		OldPrepareRenderThread &setScale(const glm::vec3 &v, const PrepareKey &key);
//		OldPrepareRenderThread &setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
//		OldPrepareRenderThread &setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
//		OldPrepareRenderThread &setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
//		OldPrepareRenderThread &updateGeometry(
//			const PrepareKey &id
//			, const Vector<SubMeshInstance> &meshs
//			, const Vector<MaterialInstance> &materials
//			, const gl::Key<AGE::AnimationInstance> &animation);
//		OldPrepareRenderThread &setCameraInfos(const PrepareKey &id, const glm::mat4 &projection);
//
//	private:
//		OctreeNode *_octree;
//
//		std::weak_ptr<AScene> scene;
//
//		Vector<Mesh> _meshs;
//		Vector<Drawable> _drawables;
//		Vector<Camera> _cameras;
//		Vector<PointLight> _pointLights;
//		Queue<PrepareKey::OctreeObjectId> _freeDrawables;
//		Queue<PrepareKey::OctreeObjectId> _freeCameras;
//		Queue<PrepareKey::OctreeObjectId> _freeMeshs;
//		Queue<PrepareKey::OctreeObjectId> _freePointLights;
//		std::size_t _MeshCounter = 0;
//		std::size_t _cameraCounter = 0;
//		std::size_t _pointLightCounter = 0;
//
//		AGE::Vector<DrawableCollection> _octreeDrawList;
//
//		virtual bool _init();
//		virtual bool _initInNewThread();
//		virtual bool _release();
//		virtual bool _releaseInNewThread();
//		virtual bool _updateBegin();
//		virtual bool _updateEnd();
//		DRAWABLE_ID addDrawable(USER_OBJECT_ID uid);
//		void removeDrawableObject(DRAWABLE_ID id);
//		void _run();
//	};
//
//	namespace Threads
//	{
//		typedef OldPrepareRenderThread Prepare;
//	}
//}