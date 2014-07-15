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

class AScene;

namespace AGE
{
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
			BoundingInfos bounding;
			CULLABLE_ID id;
			bool active;
			gl::Key<gl::Vertices> glvertices;
			gl::Key<gl::Indices> glindices;

			//TEMP
			USER_OBJECT_ID userObjectId;
		};

		struct Command
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			std::vector<BoundingInfos> boundings;
			std::vector<gl::Key<gl::Vertices>> glvertices;
			std::vector<gl::Key<gl::Indices>> glindices;
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
				, const std::vector<gl::Key<gl::Vertices>> &_glvertices
				, const std::vector<gl::Key<gl::Indices>> &_glindices
				, const std::vector<BoundingInfos> &_boundings
				, CommandType _cmdType)
				: position(0)
				, scale(0)
				, orientation(0,0,0,1)
				, glvertices(_glvertices)
				, glindices(_glindices)
				, boundings(_boundings)
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
			, const std::vector<gl::Key<gl::Vertices>> &glvertices
			, const std::vector<gl::Key<gl::Indices>> &glindices
			, const std::vector<BoundingInfos> &boundings);

		void update();

		//TMP HARDCODED
		//
		struct ToDraw
		{
			gl::Key<gl::Vertices> glvertices;
			gl::Key<gl::Indices> glindices;
			glm::mat4 transformation;
		};

		std::vector<ToDraw> TO_DRAW;
		//
		// END
	private:
		CULLABLE_ID addCullableObject(Octree::USER_OBJECT_ID uid);
		void removeCullableObject(CULLABLE_ID id);
	};
}