#include "EntityManager.hpp"
#include <imgui/imgui.h>
#include <Components/EntityRepresentation.hpp>

namespace AGE
{
	namespace WE
	{
			EntityManager::EntityManager(std::weak_ptr<AScene> &&scene)
				: System(std::move(scene))
				, _filter(std::move(scene))
			{
				_name = "we_entity_manager";
			}
			EntityManager::~EntityManager(){}

			void EntityManager::updateBegin(double time)
			{}

			void EntityManager::updateEnd(double time)
			{}

			void EntityManager::mainUpdate(double time)
			{
				float t = static_cast<float>(time);
				auto scene = _scene.lock();
				EntityFilter::Lock lock(_filter);
				for (auto &e : _filter.getCollection())
				{
					auto cpt = scene->getComponent<AGE::WE::EntityRepresentation>(e);

					ImGui::InputText("Entity %i", cpt->name.data(), cpt->name.size());
					//ImGui::TreeNode(e)
					//ImGui::TreeNode()
					//ImGui::TreePop()
				}
			}

			bool EntityManager::initialize()
			{
				_filter.setOnAdd(std::function<void(Entity e)>([&](Entity e){
					_scene.lock()->addComponent<AGE::WE::EntityRepresentation>(e, std::string("Entity " + std::to_string(e.getId())).c_str());
				}));

				for (auto i = 0; i < 30; ++i)
					_scene.lock()->createEntity();
				return true;
			}
	}
}