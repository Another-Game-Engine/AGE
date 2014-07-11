#include "Octree.hpp"
#include <Utils/Frustum.hpp>

#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>
#include <Core/EntityFilter.hpp>
#include <Components/CameraComponent.hpp>

using namespace AGE;

		Octree::USER_OBJECT_ID Octree::addElement(COMPONENT_ID componentType, const Entity &entity)
		{
			USER_OBJECT_ID res = USER_OBJECT_ID(-1);
			UserObject *ue = nullptr;
			if (!_freeUserObjects.empty())
			{
				res = _freeUserObjects.top();
				_freeUserObjects.pop();
				ue = &(_userObjects[res]);
			}
			else
			{
				res = _userObjects.size();
				_userObjects.emplace_back(UserObject());
				ue = &(_userObjects.back());
			}

			ue->id = res;
			ue->commandType = componentType;
			ue->entity = entity;
			ue->active = true;
			ue->collection.clear();
			return res;
		}

		void Octree::removeElement(std::size_t id)
		{
			_freeUserObjects.push(id);
			_userObjects[id].active = false;
			assert(id != (std::size_t)(-1));
			//todo, remove from tree
		}

		void Octree::pushCommand(UserObject &ue, CommandType cm)
		{
			if (ue.commandType.none())
				_commands.push(ue.id);
			ue.commandType.set(cm, true);			
		}

		void Octree::setPosition(const glm::vec3 &v, USER_OBJECT_ID id)
		{
			auto &ue = _userObjects[id];
			pushCommand(ue, CommandType::Position);
			ue.position = v;
		}
		void Octree::setOrientation(const glm::quat &v, USER_OBJECT_ID id)
		{
			auto &ue = _userObjects[id];
			pushCommand(ue, CommandType::Orientation);
			ue.orientation = v;
		}

		void Octree::setScale(const glm::vec3 &v, USER_OBJECT_ID id)
		{
			auto &ue = _userObjects[id];
			pushCommand(ue, CommandType::Scale);
			ue.scale = v;
		}

		void Octree::setPosition(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> ids)
		{
			for (auto &e : ids)
				setPosition(v, e);
		}

		void Octree::setOrientation(const glm::quat &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> ids)
		{
			for (auto &e : ids)
				setOrientation(v, e);
		}

		void Octree::setScale(const glm::vec3 &v, const std::array<USER_OBJECT_ID, MAX_CPT_NUMBER> ids)
		{
			for (auto &e : ids)
				setScale(v, e);
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