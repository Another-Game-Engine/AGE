#include "EntityManager.hpp"
#include <imgui/imgui.h>
#include <Components/EntityRepresentation.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Components/ComponentRegistrar.hpp>

//FOR TEST ! TO REMOVE
#include <Components/Light.hh>

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
				for (auto e : _filter.getCollection())
				{
					auto cpt = e.getComponent<AGE::WE::EntityRepresentation>();
					if (ImGui::TreeNode((void*)(cpt), cpt->name.data()))
					{
						ImGui::InputText("Name", cpt->name.data(), cpt->name.size());
						if (ImGui::SliderFloat3("Position", glm::value_ptr(cpt->position), -1000000, 1000000))
						{
							e.getLink().setPosition(cpt->position);
						}
						if (ImGui::SliderFloat3("Rotation", glm::value_ptr(cpt->rotation), -360, 360))
						{
							// temporary TODO
							//cpt->rotation.x = std::fmod(cpt->rotation.x, 360); cpt->rotation.y = std::fmod(cpt->rotation.y, 360); cpt->rotation.z = std::fmod(cpt->rotation.z, 360);
							e.getLink().setOrientation(glm::quat(cpt->rotation));
						}
						if (ImGui::SliderFloat3("Scale", glm::value_ptr(cpt->scale), 0.0f, 1000000))
						{
							e.getLink().setScale(cpt->scale);
						}

						for (ComponentType i = 0; i < MAX_CPT_NUMBER; ++i)
						{
							if (e.haveComponent(i))
							{
								auto ptr = e.getComponent(i);
								if (ptr->exposedInEditor)
								{
									if (ImGui::TreeNode(ComponentRegistrar::getInstance().getComponentName(ptr->getType()).c_str()))
									{
										ptr->editorUpdate(scene.get());
										if (ptr->deletableInEditor)
										{
											ImGui::PushID(i);
											if (ImGui::Button("Delete"))
											{
												ptr->editorDelete(scene.get());
												e.removeComponent(i);
											}
											ImGui::PopID();
										}
										ImGui::TreePop();
									}
								}
							}
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
					e.addComponent<AGE::WE::EntityRepresentation>(std::string("Entity " + std::to_string(e.getId()) + "\0").c_str());
				}));

				for (auto i = 0; i < 30; ++i)
				{
					auto e = _scene.lock()->createEntity();
					e.addComponent<PointLightComponent>()->set(glm::vec3((float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f, (float)(rand() % 1000) / 1000.0f), glm::vec3(1.f, 0.1f, 0.0f));
				}
				return true;
			}
	}
}