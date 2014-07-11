#include "Octree.hpp"
#include <Utils/Frustum.hpp>

#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>
#include <Core/EntityFilter.hpp>
#include <Components/CameraComponent.hpp>

using namespace AGE;

		Octree::USER_OBJECT_ID Octree::addElement(COMPONENT_ID componentType, const Entity &entity)
		{
			if (!_freeUserObjects.empty())
			{
				auto res = _freeUserObjects.top();
				_freeUserObjects.pop();
				_userObjects[res].componentType = componentType;
				_userObjects[res].entity = entity;
				_userObjects[res].active = true;
				return res;
			}
			auto res = _userObjects.size();
			_userObjects.emplace_back(UserObject());
			auto &ue = _userObjects.back();
			ue.id = res;
			ue.commandType = componentType;
			ue.entity = entity;
			ue.active = true;
			return res;
		}

		void Octree::removeElement(std::size_t id)
		{
			_freeUserObjects.push(id);
			_userObjects[id].active = false;
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

			static EntityFilter *filter = nullptr;
			if (!filter)
			{
				filter = new EntityFilter(std::move(scene));
				filter->requireComponent<Component::CameraComponent>();
			}

			if (filter->getCollection().size() == 0)
				return;
			auto cameraEntity = *(filter->getCollection().begin());
			auto camera = scene.lock()->getComponent<Component::CameraComponent>(cameraEntity);
			auto projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
			auto trans = glm::mat4(1);
			trans = glm::translate(trans, glm::vec3(0, 0, -40));
			auto lookAtTransform = glm::mat4(1);

			Frustum frustum;
			static float rot = 0.0f;
			rot += 1.0f;
			auto m = camera->projection * camera->lookAtTransform;
			frustum.setMatrix(m, true);
			std::uint64_t drawed = 0;
			std::uint64_t total = 0;
			for (auto &e : _elements)
			{
				if (e.active)
					++total;
				if (e.active && frustum.pointIn(e.position) == true)
				{
					//std::cout << e.entity.getId() << "  ";
					auto cpt = scene.lock()->getComponent(e.entity, e.componentTypeId);
					if (cpt)
					{
						auto c = dynamic_cast<AGE::ComponentBehavior::Cullable*>(cpt);
						c->draw = true;
						++drawed;
					}
					else
					{
						assert(false);
					}
				}
			}
			//std::cout << "Drawed : " << drawed << " / " << total << std::endl;
		}