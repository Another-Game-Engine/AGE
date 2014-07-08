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
		struct Command
		{
			glm::vec3 position;
			glm::vec3 scale;
			glm::quat orientation;
			std::size_t pushedId;
		};

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
			Command command;
		};
	private:
		std::size_t _updateId; 
		std::vector<Element> _elements;
		std::stack<std::size_t> _commands;
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
			assert(id != (std::size_t)(-1));
			//todo, remove from tree
		}

		void pushCommand(const glm::vec3 &position
			, const glm::vec3 &scale
			, const glm::quat &orientation
			, std::size_t id)
		{
			auto &element = _elements[id];
			auto &command = element.command;

			command.position = position;
			command.orientation = orientation;
			command.scale = scale;
			if (_updateId != command.pushedId)
			{
				element.command.pushedId = _updateId;
				_commands.push(id);
			}
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

		void update()
		{
			++_updateId;
			while (!_commands.empty())
			{
				//process command
				_commands.pop();
			}
		}
	};
}