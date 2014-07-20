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

#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Core/Drawable.hh>

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
			USER_OBJECT_ID  id;
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
			std::vector<SubMeshInstance> submeshInstances;
			std::vector<MaterialInstance> materialInstances;
			USER_OBJECT_ID id;
			CommandType commandType;
			BoundingInfos boundingInfo;
			glm::mat4 projection;

			OctreeCommand()
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, id(USER_OBJECT_ID(-1))
				, commandType(CommandType::END)
			{}

			OctreeCommand(USER_OBJECT_ID _id, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, id(_id)
				, commandType(_cmdType)
			{
			}

			OctreeCommand(USER_OBJECT_ID _id, const glm::vec3& pos, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, id(_id)
				, commandType(_cmdType)
			{
				if (_cmdType == CommandType::Position)
					position = pos;
				else
					scale = pos;
			}
			OctreeCommand(USER_OBJECT_ID _id, const glm::quat& ori, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(ori)
				, id(_id)
				, commandType(_cmdType)
			{
			}
			OctreeCommand(USER_OBJECT_ID _id, const glm::mat4& projection, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, id(_id)
				, commandType(_cmdType)
				, projection(projection)
			{
			}

			OctreeCommand(USER_OBJECT_ID _id
				, std::vector<SubMeshInstance> _submeshInstances // copy
				, std::vector<MaterialInstance> _materialInstances // copy
				, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
				, id(_id)
				, commandType(_cmdType)
			{}

		};

		struct UserObject
		{
			std::vector<DRAWABLE_ID> drawableCollection;
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
		std::vector<UserObject> _userObjects;
		AGE::Queue<std::size_t> _freeUserObjects;
		std::vector<CullableObject> _cullableObjects;
		AGE::Queue<std::size_t> _freeCullableObjects;
		std::vector<CameraObject> _cameraObjects;
		AGE::Queue<std::size_t> _freeCameraObjects;
		std::size_t _userObjectCounter = 0;
		std::size_t _cameraCounter = 0;

		AGE::Queue<OctreeCommand> _commandsBuffer[2];
		AGE::Queue<OctreeCommand> *_octreeCommands;
		AGE::Queue<OctreeCommand> *_mainThreadCommands;

		Queue<DrawableCollection> _drawLists[2];
		AGE::Queue<DrawableCollection> *_octreeDrawList;
		AGE::Queue<DrawableCollection> *_mainThreadDrawList;

	public:
		USER_OBJECT_ID addElement();
		void removeElement(USER_OBJECT_ID id);
		USER_OBJECT_ID addCamera();
		void removeCamera(USER_OBJECT_ID id);


		void setPosition(const glm::vec3 &v, USER_OBJECT_ID id);
		void setOrientation(const glm::quat &v, USER_OBJECT_ID id);
		void setScale(const glm::vec3 &v, USER_OBJECT_ID id);

		void setPosition(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> &ids);
		void setOrientation(const glm::quat &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> &ids);
		void setScale(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> &ids);

		void updateGeometry(USER_OBJECT_ID id
			, const std::vector<AGE::SubMeshInstance> &meshs
			, const std::vector<AGE::MaterialInstance> &materials);

		void setCameraInfos(USER_OBJECT_ID id
			, const glm::mat4 &projection);

		void update();
		AGE::Queue<DrawableCollection> *getDrawableList() { return _mainThreadDrawList; }
		//
		// END
	private:
		DRAWABLE_ID addDrawableObject(Octree::USER_OBJECT_ID uid);
		void removeDrawableObject(DRAWABLE_ID id);
	};
}