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
	struct Drawable;

	class PrepareRenderThread : public ThreadQueue, public Dependency<PrepareRenderThread>
	{
	public:
		typedef std::uint64_t USER_OBJECT_ID;

	private:
		typedef std::uint64_t DRAWABLE_ID;

	private:
		struct CullableObject
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			DRAWABLE_ID id;
			bool active;
			SubMeshInstance mesh;
			MaterialInstance material;
			bool hasMoved;
			BoundingInfos boundingInfo;
			glm::mat4 transformation;
		};

		struct CameraObject
		{
			PrepareKey key;
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			bool active;
			bool hasMoved;
			glm::mat4 projection;
		};

		struct UserObject
		{
			AGE::Vector<DRAWABLE_ID> drawableCollection;
			USER_OBJECT_ID id;
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			bool active;
		};


	private:
		//TMP
		std::weak_ptr<AScene> scene;
	public:
		void setScene(std::weak_ptr<AScene> _scene) { scene = _scene; }
	public:
		PrepareRenderThread();
		virtual ~PrepareRenderThread(void);
	private:
		AGE::Vector<UserObject> _userObjects;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeUserObjects;
		AGE::Vector<CullableObject> _cullableObjects;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeCullableObjects;
		AGE::Vector<CameraObject> _cameraObjects;
		AGE::Queue<PrepareKey::OctreeObjectId> _freeCameraObjects;
		std::size_t _userObjectCounter = 0;
		std::size_t _cameraCounter = 0;

		AGE::Vector<DrawableCollection> _octreeDrawList;

	public:
		PrepareKey addCullableElement();
		PrepareKey addCameraElement();

		void removeElement(const PrepareKey &key);

		void setPosition(const glm::vec3 &v, const PrepareKey &key);
		void setOrientation(const glm::quat &v, const PrepareKey &key);
		void setScale(const glm::vec3 &v, const PrepareKey &key);

		void setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		void setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		void setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);

		void updateGeometry(const PrepareKey &id
			, const AGE::Vector<AGE::SubMeshInstance> &meshs
			, const AGE::Vector<AGE::MaterialInstance> &materials);

		void setCameraInfos(const PrepareKey &id
			, const glm::mat4 &projection);

		//
		// END
	private:
		bool virtual _init();
		bool virtual _update();
		bool virtual _initInNewThread();
		bool virtual _release();
		bool virtual _releaseInNewThread();
		DRAWABLE_ID addDrawableObject(PrepareRenderThread::USER_OBJECT_ID uid);
		void removeDrawableObject(DRAWABLE_ID id);
		void _run();
	};

	namespace Threads
	{
		typedef PrepareRenderThread Prepare;
	}
}