#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Entities/EntityTypedef.hpp>

#include <Utils/Dependency.hpp>
#include <Utils/BoundingInfos.hpp>

#include <stack>

namespace AGE
{
	class Octree : public Dependency<Octree>
	{
	public:
		struct Element
		{
		public:
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			AGE::BoundingInfos boundings;
			ENTITY_ID entityId;
			COMPONENT_ID componentTypeId;
			std::size_t id;
			std::size_t updateId;
		};

		struct Command
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			std::size_t id;
		};

	private:
		std::size_t _updateId; 
		std::vector<Element> _elements;
		std::stack<Command> _commands;
		std::stack<std::size_t> _free;
	public:
		std::size_t addElement(COMPONENT_ID componentType)
		{
			if (!_free.empty())
			{
				auto res = _free.top();
				_free.pop();
				_elements[res].componentTypeId = componentType;
				return res;
			}
			auto res = _elements.size();
			_elements.emplace_back(Element());
			_elements.back().id = res;
			_elements.back().componentTypeId = componentType;
			return res;
		}

		void removeElement(std::size_t id)
		{
			_free.push(id);
			//todo, remove from tree
		}

		void pushCommand(const glm::vec3 &position
			, const glm::vec3 &scale
			, const glm::quat &orientation
			, std::size_t id)
		{
			_commands.push(Command());
			_commands.top().id = id;
			_commands.top().position = position;
			_commands.top().scale = scale;
			_commands.top().orientation = orientation;
		}

		void pushCommand(const glm::vec3 &position
			, const glm::vec3 &scale
			, const glm::quat &orientation
			, const std::array<std::size_t, MAX_CPT_NUMBER> _cullableLinks)
		{
			std::size_t max = (std::size_t)(-1);
			for (auto &l : _cullableLinks)
			{
				if (l == max)
					return;
				pushCommand(position, scale, orientation, l);
			}
		}
	};
}