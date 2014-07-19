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

class AScene;

namespace AGE
{
	class Drawable;

	class Octree : public Dependency<Octree>
	{
		enum CommandType
		{
			Position = 0
			, Orientation
			, Scale
			, Geometry // Update geo, material and bounding box
			, Create
			, Delete
			, END // <- should always be the last
		};

		typedef std::uint64_t CULLABLE_ID;
		typedef std::uint64_t USER_OBJECT_ID;

	private:
		struct CullableObject
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			CULLABLE_ID id;
			bool active;
			SubMeshInstance mesh;
			MaterialInstance material;
			bool hasMoved;
			glm::mat4 transformation;
		};

		struct Command
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			std::vector<SubMeshInstance> submeshInstances;
			std::vector<MaterialInstance> materialInstances;
			USER_OBJECT_ID id;
			Entity entity;
			COMPONENT_ID componentType;
			CommandType commandType;

			Command()
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, id(USER_OBJECT_ID(-1))
				, componentType(COMPONENT_ID(-1))
				, commandType(CommandType::END)
			{}

			Command(USER_OBJECT_ID _id, COMPONENT_ID _componentType, const Entity &_entity, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, id(_id)
				, entity(_entity)
				, componentType(_componentType)
				, commandType(_cmdType)
			{
			}

			Command(USER_OBJECT_ID _id, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, id(_id)
				, componentType(0)
				, commandType(_cmdType)
			{
			}

			Command(USER_OBJECT_ID _id, const glm::vec3& pos, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0, 0, 0, 1)
				, id(_id)
				, componentType(0)
				, commandType(_cmdType)
			{
				if (_cmdType == CommandType::Position)
					position = pos;
				else
					scale = pos;
			}
			Command(USER_OBJECT_ID _id, const glm::quat& ori, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(ori)
				, id(_id)
				, componentType(0)
				, commandType(_cmdType)
			{
			}

			Command(USER_OBJECT_ID _id
				, std::vector<SubMeshInstance> _submeshInstances // copy
				, std::vector<MaterialInstance> _materialInstances // copy
				, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
				, id(_id)
				, componentType(0)
				, commandType(_cmdType)
			{}

		};

		struct UserObject
		{
			std::vector<CULLABLE_ID> collection;
			Entity entity;
			COMPONENT_ID componentType;
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
		std::size_t _userObjectCounter = 0;

		AGE::Queue<Command> _commandsBuffer[2];
		AGE::Queue<Command> *_octreeCommands;
		AGE::Queue<Command> *_mainThreadCommands;
	public:
		USER_OBJECT_ID addElement(COMPONENT_ID componentType, const Entity &entity);
		void removeElement(USER_OBJECT_ID id);

		void setPosition(const glm::vec3 &v, USER_OBJECT_ID id);
		void setOrientation(const glm::quat &v, USER_OBJECT_ID id);
		void setScale(const glm::vec3 &v, USER_OBJECT_ID id);

		void setPosition(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> &ids);
		void setOrientation(const glm::quat &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> &ids);
		void setScale(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> &ids);

		void updateGeometry(USER_OBJECT_ID id
			, const std::vector<AGE::SubMeshInstance> &meshs
			, const std::vector<AGE::MaterialInstance> &materials);

		void update();

		Queue<Drawable> drawList;
		//
		// END
	private:
		CULLABLE_ID addCullableObject(Octree::USER_OBJECT_ID uid);
		void removeCullableObject(CULLABLE_ID id);
	};
}