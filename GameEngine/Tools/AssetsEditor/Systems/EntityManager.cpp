#include "EntityManager.hpp"
#include <imgui/imgui.h>
#include <Components/EntityRepresentation.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Components/ComponentRegistrationManager.hpp>
#include <Components/MeshRenderer.hh>
#include <AssetManagement/AssetManager.hh>

namespace AGE
{
	namespace WE
	{
			EntityManager::EntityManager(AScene *scene)
				: System(std::move(scene))
				, _filter(std::move(scene))
				, _meshRenderers(std::move(scene))
				, _selectedEntity(0)
			{
				auto name = "../../MyScene\0";
				memcpy(_sceneName, name, strlen(name) + 1);
				_meshRenderers.requireComponent<MeshRenderer>();
			}
			EntityManager::~EntityManager(){}

			void EntityManager::updateBegin(float time)
			{}

			void EntityManager::updateEnd(float time)
			{}

			void EntityManager::mainUpdate(float time)
			{
				ImGui::BeginChild("Entity list", ImVec2(ImGui::GetWindowWidth() * 0.25f, 0));

				float t = static_cast<float>(time);
				auto scene = _scene;
				EntityFilter::Lock lock(_filter);

				// Disgusting but fuck it ! :)
				_entityNames.clear();
				_entities.clear();
				{
					EntityFilter::Lock lock(_filter);
					for (auto e : _filter.getCollection())
					{
						auto representation = e.getComponent<AGE::WE::EntityRepresentation>();
						if (representation->editorOnly)
						{
							_filter.manuallyRemoveEntity(e);
							continue;
						}
						_entityNames.push_back(representation->name);
						_entities.push_back(e);
					}
				}
				if (_selectedEntity >= _entities.size())
					_selectedEntity = 0;

				if (_entities.size() > 0)
				{
					ImGui::PushItemWidth(-1);
					//ImGui::ListBoxHeader("##empty");
					ImGui::ListBox("##empty", &_selectedEntity, &(_entityNames.front()), (int)(_entityNames.size()));
					//ImGui::ListBoxFooter();
					ImGui::PopItemWidth();

					ImGui::Separator();
				}

				ImGui::BeginChild("Edit entity");

				if (_entities.size() > 0 && _selectedEntity < _entities.size())
				{
					auto e = _entities[_selectedEntity];
					auto cpt = e.getComponent<AGE::WE::EntityRepresentation>();

					ImGui::InputText("Name", cpt->name, ENTITY_NAME_LENGTH);
					cpt->position = e.getLink().getPosition();
					if (ImGui::InputFloat3("Position", glm::value_ptr(cpt->position)))
					{
						e.getLink().setPosition(cpt->position);
					}

					cpt->rotation = glm::eulerAngles(e.getLink().getOrientation());
					if (ImGui::InputFloat3("Rotation", glm::value_ptr(cpt->rotation)))
					{
						e.getLink().setOrientation(glm::quat(cpt->rotation));
					}

					cpt->scale = e.getLink().getScale();
					if (ImGui::InputFloat3("Scale", glm::value_ptr(cpt->scale)))
					{
						e.getLink().setScale(cpt->scale);
					}

					ImGui::Separator();

					auto &components = e.getComponentList();
					for (ComponentType i = 0; i < components.size(); ++i)
					{
						if (e.haveComponent(i))
						{
							auto ptr = e.getComponent(i);
							if (ptr->exposedInEditor)
							{
								if (ImGui::TreeNode(ComponentRegistrationManager::getInstance().getComponentName(ptr->getType()).c_str()))
								{
									ptr->editorUpdate(scene);
									if (ptr->deletableInEditor)
									{
										ImGui::PushID(i);
										if (ImGui::Button("Delete"))
										{
											ptr->editorDelete(scene);
											e.removeComponent(i);
										}
										ImGui::PopID();
									}
									ImGui::TreePop();
								}
							}
						}
					}

					ImGui::Separator();

					auto &types = ComponentRegistrationManager::getInstance().getSystemIdToAgeIdMap();
					auto &creationFn = ComponentRegistrationManager::getInstance().getCreationFunctions();

					for (auto &t : types)
					{
						if (!e.haveComponent(t.second))
						{
							if (ImGui::Button(std::string("Add : " + ComponentRegistrationManager::getInstance().getComponentName(t.second)).c_str()))
							{
								creationFn.at(t.first)(&e);
							}
						}
					}

					ImGui::Separator();

					if (ImGui::Button("Delete entity"))
					{
						_scene->destroy(e);
					}
				}

				
				if (ImGui::Button("Create entity"))
				{
					_scene->createEntity();
				}

				ImGui::InputText("Scene file name", _sceneName, MAX_SCENE_NAME_LENGTH);

				if (ImGui::Button("Save scene"))
				{
					WESerialization::SetSerializeForEditor(true);
					// we list all assets dependencies
					{
						AssetsManager::AssetsPackage package;
						for (auto e : _meshRenderers.getCollection())
						{
							auto cpt = e.getComponent<MeshRenderer>();
							if (!cpt->selectedMeshPath.empty())
							{
								package.meshs.insert(cpt->selectedMeshPath);
							}
							if (!cpt->selectedMaterialPath.empty())
							{
								package.materials.insert(cpt->selectedMaterialPath);
							}
						}
						_scene->getInstance<AssetsManager>()->savePackage(package, std::string(_sceneName) + "_assets.json");
					}

					scene->saveSelectionToJson(std::string(_sceneName) + ".json", _filter.getCollection());
					WESerialization::SetSerializeForEditor(false);
				}
				if (ImGui::Button("Export scene"))
				{
					WESerialization::SetSerializeForEditor(false);
					// we list all assets dependencies
					{
						AssetsManager::AssetsPackage package;
						for (auto e : _meshRenderers.getCollection())
						{
							auto cpt = e.getComponent<MeshRenderer>();
							if (!cpt->selectedMeshPath.empty())
							{
								package.meshs.insert(cpt->selectedMeshPath);
							}
							if (!cpt->selectedMaterialPath.empty())
							{
								package.materials.insert(cpt->selectedMaterialPath);
							}
						}
						_scene->getInstance<AssetsManager>()->savePackage(package, std::string(_sceneName) + "_assets.json");
					}

					scene->saveSelectionToJson(std::string(_sceneName) + "_export.json", _filter.getCollection());
				}
				if (ImGui::Button("Load scene"))
				{
					WESerialization::SetSerializeForEditor(true);

					auto sceneFileName = std::string(_sceneName) + ".json";
					auto assetPackageFileName = std::string(_sceneName) + "_assets.json";

					_scene->getInstance<AssetsManager>()->loadPackage(assetPackageFileName, assetPackageFileName);

					int totalToLoad = 0;
					int toLoad = 0;
					std::string loadingError;

					std::cout << "Loading assets";
					do {
						scene->getInstance<AGE::AssetsManager>()->updateLoadingChannel(assetPackageFileName, totalToLoad, toLoad, loadingError);
						std::this_thread::sleep_for(std::chrono::milliseconds(300));
						std::cout << ".";
					} while
						(toLoad > 0 && loadingError.size() == 0);
					std::cout << std::endl;
					scene->loadFromJson(sceneFileName);
					WESerialization::SetSerializeForEditor(false);
				}

				ImGui::EndChild();

				ImGui::EndChild(); // Entity List
			}

			bool EntityManager::initialize()
			{
				_filter.setOnAdd(std::function<void(Entity e)>([this](Entity en)
				{
					en.addComponent<AGE::WE::EntityRepresentation>(std::string("Entity " + std::to_string(en.getId()) + "\0").c_str());
				}));

				_filter.setOnRemove(std::function<void(Entity e)>([this](Entity en)
				{
				}));
				return true;
			}
	}
}