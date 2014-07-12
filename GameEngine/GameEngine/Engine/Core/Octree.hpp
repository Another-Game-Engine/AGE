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

#include <queue>
#include <stack>

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
			
			std::bitset<CommandType::END> commandType;
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
		Octree()
		{
		}

		virtual ~Octree(void)
		{
		}

	private:
		std::vector<UserObject> _userObjects;
		std::stack<std::size_t> _freeUserObjects;
		std::vector<CullableObject> _cullableObjects;
		std::stack<std::size_t> _freeCullableObjects;
		std::size_t _userObjectCounter = 0;

		std::queue<Command> _commands;
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
	private:
		CULLABLE_ID addCullableObject(Octree::USER_OBJECT_ID uid);
		void removeCullableObject(CULLABLE_ID id);
	};
}