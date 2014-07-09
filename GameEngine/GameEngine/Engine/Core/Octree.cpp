#include "Octree.hpp"
#include <Utils/Frustum.hpp>

#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>

using namespace AGE;

		std::size_t Octree::addElement(COMPONENT_ID componentType, const Entity &entity)
		{
			if (!_free.empty())
			{
				auto res = _free.top();
				_free.pop();
				_elements[res].componentTypeId = componentType;
				_elements[res].entity = entity;
				_elements[res].active = true;
				return res;
			}
			auto res = _elements.size();
			_elements.emplace_back(Element());
			_elements.back().id = res;
			_elements.back().componentTypeId = componentType;
			_elements.back().entity = entity;
			_elements.back().active = true;
			return res;
		}

		void Octree::removeElement(std::size_t id)
		{
			_free.push(id);
			_elements[id].active = false;
			assert(id != (std::size_t)(-1));
			//todo, remove from tree
		}

		void Octree::pushCommand(const glm::vec3 &position
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

		void Octree::pushCommand(const glm::vec3 &position
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

		void Octree::update()
		{
			while (!_commands.empty())
			{
				//process command
				auto &element = _elements[_commands.top()];
				auto &command = element.command;
				element.orientation = command.orientation;
				element.position = command.position;
				element.scale = command.scale;
				_commands.pop();
			}

			auto projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
			auto trans = glm::mat4(1);
			trans = glm::translate(trans, glm::vec3(0, 0, -40));
			auto lookAtTransform = glm::mat4(1);

			Frustum frustum;
			frustum.setMatrix(projection, true);

			for (auto &e : _elements)
			{
				if (e.active && frustum.pointIn(e.position) != true)
				{
					std::cout << e.entity.getId() << "  ";
					auto cpt = scene->getComponent(e.entity, e.componentTypeId);
					if (cpt)
					{
						((AGE::ComponentBehavior::Cullable*)(cpt))->draw = true;
					}
					else
					{
						assert(false);
					}
				}
			}
		}