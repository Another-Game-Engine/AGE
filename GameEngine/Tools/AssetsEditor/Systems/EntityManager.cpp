#include "EntityManager.hpp"
#include <imgui/imgui.h>
#include <Components/EntityRepresentation.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Components/ComponentRegistrationManager.hpp>
#include <Components/MeshRenderer.hh>
#include <AssetManagement/AssetManager.hh>
#include <EditorConfiguration.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/FreeFlyComponent.hh>
#include <Entities/Archetype.hpp>

namespace AGE
{
	namespace WE
	{
			EntityManager::EntityManager(AScene *scene)
				: System(std::move(scene))
				, _filter(std::move(scene))
				, _meshRenderers(std::move(scene))
				, _selectedEntity(nullptr)
				, _selectedEntityIndex(0)
				, _graphNodeDisplay(false)
				, _selectParent(false)
			{
				//
				auto name = "\0";
				strcpy_s(_sceneName, name);
				strcpy_s(_exportName, name);
				_meshRenderers.requireComponent<MeshRenderer>();

				generateBasicEntities();
			}
			EntityManager::~EntityManager(){}

			void EntityManager::updateBegin(float time)
			{}

			void EntityManager::updateEnd(float time)
			{}

			void EntityManager::mainUpdate(float time)
			{
				ImGui::BeginChild("Entity list", ImVec2(ImGui::GetWindowWidth() * 0.35f, 0));

				ImGui::Checkbox("Graphnode display", &_graphNodeDisplay);

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

						if (_graphNodeDisplay)
						{
							// if it's not attached to root
							if (e.getLink().hasParent())
							{
								continue;
							}
						}
						_entityNames.push_back(representation->name);
						_entities.push_back(e);
					}
				}
				if (_selectedEntityIndex >= _entities.size())
				{
					_selectedEntityIndex = 0;
				}

				if (_entities.size() > 0 && !_graphNodeDisplay)
				{
					ImGui::PushItemWidth(-1);
					//ImGui::ListBoxHeader("##empty");
					if (ImGui::ListBox("##empty", &_selectedEntityIndex, &(_entityNames.front()), (int)(_entityNames.size())))
					{
						if (_entities.size() > 0 && _selectedEntityIndex < _entities.size())
						{
							_selectedEntity = _entities[_selectedEntityIndex].getPtr();
						}
						else
						{
							_selectedEntity = nullptr;
						}
					}
					//ImGui::ListBoxFooter();
					ImGui::PopItemWidth();
				}
				else
				{
					for (auto &e : _entities)
					{
						recursiveDisplayList(e);
					}
				}

				ImGui::Separator();

				ImGui::BeginChild("Edit entity");

				if (_entities.size() > 0 && _selectedEntity != nullptr)
				{
					auto entity = *_selectedEntity;
					displayEntity(entity);

					ImGui::Separator();

					auto &types = ComponentRegistrationManager::getInstance().getSystemIdToAgeIdMap();
					auto &creationFn = ComponentRegistrationManager::getInstance().getCreationFunctions();

					for (auto &t : types)
					{
						if (!entity.haveComponent(t.second))
						{
							if (ImGui::SmallButton(std::string("Add : " + ComponentRegistrationManager::getInstance().getComponentName(t.second)).c_str()))
							{
								creationFn.at(t.first)(&entity);
							}
						}
					}

					ImGui::Separator();

					if (ImGui::Button("Delete entity"))
					{
						_scene->destroy(entity);
						_selectedEntity = nullptr;
						_selectedEntityIndex = 0;
					}

					if (ImGui::Button("Duplicate"))
					{
						Entity duplicate;
						_scene->copyEntity(entity, duplicate, true, false);
					}

					ImGui::InputText("Archetype name", _archetypeName, MAX_SCENE_NAME_LENGTH);
					ImGui::SameLine();
					if (ImGui::Button("Save as archetype"))
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
							_scene->getInstance<AssetsManager>()->savePackage(package, WE::EditorConfiguration::GetEditedArchetypeDirectory() + std::string(_archetypeName) + "_assets.json");
						}

						_scene->saveSelectionToJson(WE::EditorConfiguration::GetEditedArchetypeDirectory() + std::string(_archetypeName) + "_archetype.json", _entities);
						WESerialization::SetSerializeForEditor(false);
					}
				}

				
				if (ImGui::Button("Create entity"))
				{
					_scene->createEntity();
				}

				if (ImGui::ListBox("Scenes", &WE::EditorConfiguration::getSelectedSceneIndex(), WE::EditorConfiguration::getScenesName().data(), WE::EditorConfiguration::getScenesName().size()))
				{
					_scene->clearAllEntities();

					generateBasicEntities();

					WESerialization::SetSerializeForEditor(true);

					auto sceneFileName = WE::EditorConfiguration::getSelectedScenePath() + "_scene_description.json";
					auto assetPackageFileName = WE::EditorConfiguration::getSelectedScenePath() + "_assets.json";

					strcpy_s(_sceneName, WE::EditorConfiguration::getSelectedSceneName().c_str());
					strcpy_s(_exportName, WE::EditorConfiguration::getSelectedSceneName().c_str());

					_scene->getInstance<AssetsManager>()->pushNewCallback(assetPackageFileName, std::function<void()>([=](){
						_scene->loadFromJson(sceneFileName);
						WESerialization::SetSerializeForEditor(false);
					}));
					_scene->getInstance<AssetsManager>()->loadPackage(assetPackageFileName, assetPackageFileName);
				}

				_entities.clear();
				{
					EntityFilter::Lock lock(_filter);
					for (auto e : _filter.getCollection())
					{
						// if it's not attached to root
						if (e.getLink().hasParent())
						{
							continue;
						}
						_entities.push_back(e);
					}
				}
				ImGui::InputText("File name", _sceneName, MAX_SCENE_NAME_LENGTH);
				ImGui::SameLine();
				if (ImGui::Button("Save"))
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
						_scene->getInstance<AssetsManager>()->savePackage(package, WE::EditorConfiguration::GetEditedSceneDirectory() + std::string(_sceneName) + "_assets.json");
					}

					_scene->saveSelectionToJson(WE::EditorConfiguration::GetEditedSceneDirectory() + std::string(_sceneName) + "_scene_description.json", _entities);
					WESerialization::SetSerializeForEditor(false);
				}
				ImGui::InputText("Export name", _exportName, MAX_SCENE_NAME_LENGTH);
				ImGui::SameLine();
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
						_scene->getInstance<AssetsManager>()->savePackage(package, WE::EditorConfiguration::GetExportedSceneDirectory() + std::string(_exportName) + "_assets.json");
					}

					_scene->saveSelectionToJson(WE::EditorConfiguration::GetExportedSceneDirectory() + std::string(_exportName) + "_export.json", _entities);
				}

				ImGui::EndChild();

				ImGui::EndChild(); // Entity List
			}

			void EntityManager::displayEntity(Entity &entity)
			{
				auto cpt = entity.getComponent<AGE::WE::EntityRepresentation>();

				ImGui::InputText("Name", cpt->name, ENTITY_NAME_LENGTH);
				cpt->position = entity.getLink().getPosition();
				if (ImGui::InputFloat3("Position", glm::value_ptr(cpt->position)))
				{
					entity.getLink().setPosition(cpt->position);
				}

				cpt->rotation = glm::eulerAngles(entity.getLink().getOrientation());
				if (ImGui::InputFloat3("Rotation", glm::value_ptr(cpt->rotation)))
				{
					entity.getLink().setOrientation(glm::quat(cpt->rotation));
				}

				cpt->scale = entity.getLink().getScale();
				if (ImGui::InputFloat3("Scale", glm::value_ptr(cpt->scale)))
				{
					entity.getLink().setScale(cpt->scale);
				}

				ImGui::Separator();

				auto &components = entity.getComponentList();
				for (ComponentType i = 0; i < components.size(); ++i)
				{
					if (entity.haveComponent(i))
					{
						auto ptr = entity.getComponent(i);
						if (ptr->exposedInEditor)
						{
							bool opened = ImGui::TreeNode(ComponentRegistrationManager::getInstance().getComponentName(ptr->getType()).c_str());
							bool deleted = false;
							if (ptr->deletableInEditor)
							{
								ImGui::SameLine();
								ImGui::PushID(i);
								deleted = ImGui::SmallButton("Delete");
								if (deleted)
								{
									ptr->editorDelete(_scene);
									entity.removeComponent(i);
								}
								ImGui::PopID();
							}
							if (opened && !deleted)
							{
								ptr->editorUpdate(_scene);
								ImGui::TreePop();
							}
						}
					}
				}
			}

			void EntityManager::recursiveDisplayList(Entity &entity)
			{
				auto cpt = entity.getComponent<AGE::WE::EntityRepresentation>();
				bool opened = false;
				opened = ImGui::TreeNode(cpt->name);
				ImGui::PushID(entity.getPtr());
				if (_selectedEntity != entity.getPtr())
				{
					if (!_selectParent)
					{
						ImGui::SameLine();
						if (ImGui::SmallButton("Select"))
						{
							_selectedEntity = entity.getPtr();
						}
					}
					else/* (_selectedEntity != nullptr)*/
					{
						ImGui::SameLine();
						if (ImGui::SmallButton("Set as parent"))
						{
							_selectedEntity->getLink().attachParent(entity.getLinkPtr());
							_selectParent = false;
						}
					}
				}
				else
				{
					if (!_selectParent)
					{
						ImGui::SameLine();
						if (ImGui::SmallButton("Set parent"))
						{
							_selectParent = true;
						}
					}
					else
					{
						ImGui::SameLine();
						if (ImGui::SmallButton("Root"))
						{
							_selectedEntity->getLink().detachParent();
							_selectParent = false;
						}
						ImGui::SameLine();
						if (ImGui::SmallButton("Cancel"))
						{
							_selectParent = false;
						}
					}
				}
				ImGui::PopID();
				if (opened)
				{
					if (entity.getLink().hasChildren())
					{
						auto children = entity.getLink().getChildren();
						for (auto &e : children)
						{
							recursiveDisplayList(e->getEntity()->getEntity());
						}
					}
					ImGui::TreePop();
				}
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

			void EntityManager::generateBasicEntities()
			{
				auto camera = _scene->createEntity();
				auto cam = camera.addComponent<CameraComponent>();
				cam->setPipeline(RenderType::DEBUG_DEFERRED);
				camera.getLink().setPosition(glm::vec3(0, 3, 5));
				camera.getLink().setForward(glm::vec3(0, 0, 0));
				camera.addComponent<FreeFlyComponent>();
				camera.addComponent<AGE::WE::EntityRepresentation>()->editorOnly = true;
			}
	}
}