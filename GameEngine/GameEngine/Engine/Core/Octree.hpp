#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Entities/EntityTypedef.hpp>

#include <Utils/Dependency.hpp>
#include <Utils/BoundingInfos.hpp>

#include <Entities/Entity.hh>

#include <OpenGL/Key.hh>
#include <OpenGL/Data.hh>

#include <stack>

#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>

#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Core/Drawable.hh>

#include <Core/OctreeKey.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>

class AScene;


namespace AGE
{
	class Drawable;

	class Octree : public Dependency<Octree>
	{
	public:
		typedef std::uint64_t USER_OBJECT_ID;

	private:
		enum CommandType
		{
			Position = 0
			, Orientation
			, Scale
			, Geometry // Update geo, material and bounding box
			, CreateDrawable
			, DeleteDrawable
			, CreateCamera
			, DeleteCamera
			, CameraInfos
			, END // <- should always be the last
		};

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

		struct OctreeCommand
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			AGE::Vector<SubMeshInstance> submeshInstances;
			AGE::Vector<MaterialInstance> materialInstances;
			OctreeKey key;
			CommandType commandType;
			BoundingInfos boundingInfo;
			glm::mat4 projection;

			OctreeCommand()
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, commandType(CommandType::END)
			{}

			OctreeCommand(const OctreeKey &_key, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, key(_key)
				, commandType(_cmdType)
			{
			}

			OctreeCommand(const OctreeKey &_key, const glm::vec3& pos, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, key(_key)
				, commandType(_cmdType)
			{
				if (_cmdType == CommandType::Position)
					position = pos;
				else
					scale = pos;
			}
			OctreeCommand(const OctreeKey &_key, const glm::quat& ori, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(ori)
				, key(_key)
				, commandType(_cmdType)
			{
			}
			OctreeCommand(const OctreeKey &_key, const glm::mat4& projection, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, key(_key)
				, commandType(_cmdType)
				, projection(projection)
			{
			}

			OctreeCommand(const OctreeKey &_key
				, AGE::Vector<SubMeshInstance> _submeshInstances // copy
				, AGE::Vector<MaterialInstance> _materialInstances // copy
				, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
				, key(_key)
				, commandType(_cmdType)
			{}

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

		AGE::Queue<OctreeCommand> _commandsBuffer[2];
		AGE::Queue<OctreeCommand> *_octreeCommands;
		AGE::Queue<OctreeCommand> *_mainThreadCommands;


		AGE::Vector<DrawableCollection> _octreeDrawList;
		AGE::Vector<DrawableCollection> _mainThreadDrawList;

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

		void update();
		AGE::Vector<DrawableCollection> &getDrawableList();

		//
		// END
	private:
		void _update();
		DRAWABLE_ID addDrawableObject(Octree::USER_OBJECT_ID uid);
		void removeDrawableObject(DRAWABLE_ID id);
		void _run();

		std::mutex _mutex;
		std::condition_variable _cond;
		std::thread *_thread;
	};
}