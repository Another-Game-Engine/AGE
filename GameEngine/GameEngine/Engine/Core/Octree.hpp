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
	private:
		//TMP
		std::weak_ptr<AScene> scene;
	public:
		void setScene(std::weak_ptr<AScene> _scene) { scene = _scene; }
	public:
		struct Command
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			std::size_t pushedId;
		};

		struct Element
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			AGE::BoundingInfos boundings;
			Entity entity;
			COMPONENT_ID componentTypeId;
			std::size_t id;
			std::size_t updateId;
			Command command;
			bool active = false;
		};

		Octree()
		{
		}

		virtual ~Octree(void)
		{
		}

	private:
		std::size_t _updateId; 
		std::vector<Element> _elements;
		std::stack<std::size_t> _commands;
		std::stack<std::size_t> _free;
	public:
		std::size_t addElement(COMPONENT_ID componentType, const Entity &entity);
		void removeElement(std::size_t id);
		void pushCommand(const glm::vec3 &position
			, const glm::vec3 &scale
			, const glm::quat &orientation
			, std::size_t id);
		void pushCommand(const glm::vec3 &position
			, const glm::vec3 &scale
			, const glm::quat &orientation
			, const std::array<std::size_t, MAX_CPT_NUMBER> _cullableLinks);
		void update();
	};
}