#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

#include <Core/OctreeKey.hpp>

#include <tmq/doubleBuffered/templateDispatcher.hpp>
#include <tmq/doubleBuffered/queue.hpp>

#include <thread>
#include <condition_variable>
#include <future>

class AScene;

namespace AGE
{
	struct Drawable;

	namespace OctreeCommand
	{
		struct Position
		{
			Position(const OctreeKey &_key, const glm::vec3 &_position)
				: key(_key)
				, position(_position)
			{}
			OctreeKey key;
			glm::vec3 position;
		};

		struct Scale
		{
			Scale(const OctreeKey &_key, const glm::vec3 &_scale)
				: key(_key)
				, scale(_scale)
			{}
			OctreeKey key;
			glm::vec3 scale;
		};

		struct Orientation
		{
			Orientation(const OctreeKey &_key, const glm::quat &_orientation)
				: key(_key)
				, orientation(_orientation)
			{}
			OctreeKey key;
			glm::quat orientation;
		};

		struct Geometry
		{
			Geometry(const OctreeKey &_key
				, const AGE::Vector<SubMeshInstance> &_submeshInstances
				, const AGE::Vector<MaterialInstance> &_materialInstances)
				: key(_key)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
			{}
			OctreeKey key;
			AGE::Vector<SubMeshInstance> submeshInstances;
			AGE::Vector<MaterialInstance> materialInstances;
		};

		struct CreateDrawable
		{
			CreateDrawable(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct DeleteDrawable
		{
			DeleteDrawable(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct CreateCamera
		{
			CreateCamera(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct DeleteCamera
		{
			DeleteCamera(const OctreeKey &_key)
				: key(_key)
			{}
			OctreeKey key;
		};

		struct CameraInfos
		{
			CameraInfos(const OctreeKey &_key, const glm::mat4 &_projection)
				: key(_key)
				, projection(_projection)
			{}
			OctreeKey key;
			glm::mat4 projection;
		};

		struct SwapDrawLists : public TMQ::FutureData<AGE::Vector<DrawableCollection>>
		{
		};

		struct PrepareDrawLists
		{};
	}

	class Octree : public Dependency<Octree>
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
			OctreeKey key;
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
		Octree();
		virtual ~Octree(void);
	private:
		AGE::Vector<UserObject> _userObjects;
		AGE::Queue<OctreeKey::OctreeObjectId> _freeUserObjects;
		AGE::Vector<CullableObject> _cullableObjects;
		AGE::Queue<OctreeKey::OctreeObjectId> _freeCullableObjects;
		AGE::Vector<CameraObject> _cameraObjects;
		AGE::Queue<OctreeKey::OctreeObjectId> _freeCameraObjects;
		std::size_t _userObjectCounter = 0;
		std::size_t _cameraCounter = 0;

		TMQ::Double::Queue _commandQueue;

		AGE::Vector<DrawableCollection> _octreeDrawList;

	public:
		const OctreeKey &addCullableElement();
		const OctreeKey &addCameraElement();

		void removeElement(const OctreeKey &key);


		void setPosition(const glm::vec3 &v, const OctreeKey &key);
		void setOrientation(const glm::quat &v, const OctreeKey &key);
		void setScale(const glm::vec3 &v, const OctreeKey &key);

		void setPosition(const glm::vec3 &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids);
		void setOrientation(const glm::quat &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids);
		void setScale(const glm::vec3 &v, const std::array<OctreeKey, MAX_CPT_NUMBER> &ids);

		void updateGeometry(const OctreeKey &id
			, const AGE::Vector<AGE::SubMeshInstance> &meshs
			, const AGE::Vector<AGE::MaterialInstance> &materials);

		void setCameraInfos(const OctreeKey &id
			, const glm::mat4 &projection);

		void getDrawableList(AGE::Vector<DrawableCollection> &list);
		void update();
		//
		// END
	private:
		bool _update();
		DRAWABLE_ID addDrawableObject(Octree::USER_OBJECT_ID uid);
		void removeDrawableObject(DRAWABLE_ID id);
		void _run();

		std::thread *_thread;
		bool _isRunning;
	};
}