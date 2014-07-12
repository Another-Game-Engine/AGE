#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Entities/EntityTypedef.hpp>

#include <Utils/Dependency.hpp>
#include <Utils/BoundingInfos.hpp>

#include <Entities/Entity.hh>

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
			//std::shared_ptr<geometry> geo;
			//std::shared_ptr<material> material;

			//TEMP
			USER_OBJECT_ID userObjectId;
		};

		struct UserObject
		{
			std::vector<CULLABLE_ID> collection;
			Entity entity;
			COMPONENT_ID componentType;
			USER_OBJECT_ID id;
			bool active;

			// for command
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			BoundingInfos bounding;
			std::bitset<CommandType::END> commandType;
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

		std::stack<std::size_t> _commands;
	public:
		USER_OBJECT_ID addElement(COMPONENT_ID componentType, const Entity &entity);
		void removeElement(USER_OBJECT_ID id);

		void setPosition(const glm::vec3 &v, USER_OBJECT_ID id);
		void setOrientation(const glm::quat &v, USER_OBJECT_ID id);
		void setScale(const glm::vec3 &v, USER_OBJECT_ID id);

		void setPosition(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> ids);
		void setOrientation(const glm::quat &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> ids);
		void setScale(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> ids);

		//void setGeometry(TODO)

		void update();
	private:
		void pushCommand(UserObject &ue, CommandType cm);
		CULLABLE_ID addCullableObject(Octree::USER_OBJECT_ID uid);
		void removeCullableObject(CULLABLE_ID id);
	};
}