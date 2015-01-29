#include "EntityManager.hpp"
#include <imgui/imgui.h>
#include <Components/EntityRepresentation.hpp>
#include <glm/gtc/type_ptr.hpp>

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
					if (ImGui::TreeNode((void*)(cpt), cpt->name.data()))
					{
						ImGui::InputText("Name", cpt->name.data(), cpt->name.size());
						if (ImGui::SliderFloat3("Position", glm::value_ptr(cpt->position), -1000000, 1000000))
						{
							scene->getLink(e)->setPosition(cpt->position);
						}
						if (ImGui::SliderFloat3("Rotation", glm::value_ptr(cpt->rotation), -360, 360))
						{
							// temporary TODO
							//cpt->rotation.x = std::fmod(cpt->rotation.x, 360); cpt->rotation.y = std::fmod(cpt->rotation.y, 360); cpt->rotation.z = std::fmod(cpt->rotation.z, 360);
							scene->getLink(e)->setOrientation(glm::quat(cpt->rotation));
						}
						if (ImGui::SliderFloat3("Scale", glm::value_ptr(cpt->scale), 0.0f, 1000000))
						{
							scene->getLink(e)->setScale(cpt->scale);
						}

						ImGui::TreePop();
					}
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