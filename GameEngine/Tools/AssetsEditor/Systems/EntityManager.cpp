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
#include <Managers/ArchetypesEditorManager.hpp>
#include <EntityHelpers/EntityImgui.hpp>
#include <Entities/EntityReadablePacker.hpp>
#include <Entities/ReadableEntityPack.hpp>
#include <Entities/EntityBinaryPacker.hpp>
#include <Entities/BinaryEntityPack.hpp>

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
						recursiveDisplayList(e, _selectedEntity, _selectParent);
					}
				}

				ImGui::Separator();

				ImGui::BeginChild("Edit entity");

				if (_entities.size() > 0 && _selectedEntity != nullptr)
				{
					auto entity = *_selectedEntity;
					displayEntity(entity, _scene);

					ImGui::Separator();

					auto representation = entity.getComponent<AGE::WE::EntityRepresentation>();
					auto archetypeCpt = entity.getComponent<AGE::ArchetypeComponent>();

					if (archetypeCpt == nullptr)
					{
						auto &types = ComponentRegistrationManager::getInstance().getExposedType();
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
					}

					auto isAnArchetype = archetypeCpt != nullptr;
					auto isAnArchetypeChild = isAnArchetype && archetypeCpt->parentIsAnArchetype;

					if (!isAnArchetypeChild && ImGui::SmallButton("Delete entity"))
					{
						auto destroyAllHierarchy = archetypeCpt != nullptr;
						_scene->destroy(entity, destroyAllHierarchy);
						_selectedEntity = nullptr;
						_selectedEntityIndex = 0;
					}

					if (isAnArchetypeChild == false && ImGui::SmallButton("Duplicate"))
					{
						Entity duplicate;
						_scene->copyEntity(entity, duplicate, true, false);
					}

					if (isAnArchetypeChild == false)
					{
						ImGui::InputText("Archetype name", _archetypeName, MAX_SCENE_NAME_LENGTH);
						if (ImGui::SmallButton("Convert to Archetype"))
						{
							auto manager = _scene->getInstance<AGE::WE::ArchetypeEditorManager>();
							manager->addOne(_archetypeName, *_selectedEntity);
						}
					}
				}

				
				if (ImGui::Button("Create entity"))
				{
					_scene->createEntity();
				}
				if (ImGui::TreeNode("Scenes list"))
				{
					if (ImGui::ListBox("Scenes", &WE::EditorConfiguration::getSelectedSceneIndex(), WE::EditorConfiguration::getScenesName().data(), static_cast<int>(WE::EditorConfiguration::getScenesName().size())))
					{
						_scene->clearAllEntities();

						generateBasicEntities();

						WESerialization::SetSerializeForEditor(true);

						auto sceneFileName = WE::EditorConfiguration::getSelectedScenePath() + ".raw_scene";

						strcpy_s(_sceneName, WE::EditorConfiguration::getSelectedSceneName().c_str());

						ReadableEntityPack pack;
						pack.scene = _scene;
						pack.loadFromFile(sceneFileName);
					}
					ImGui::TreePop();
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

					ReadableEntityPack pack;
					{
						CreateReadableEntityPack(pack, _entities);
						pack.saveToFile(WE::EditorConfiguration::GetEditedSceneDirectory() + std::string(_sceneName) + ".raw_scene");
					}
					{
						BinaryEntityPack binaryPack = pack.toBinary();
						binaryPack.saveToFile(WE::EditorConfiguration::GetExportedSceneDirectory() + std::string(_sceneName) + ".scene");
					}

					WESerialization::SetSerializeForEditor(false);
				}

				ImGui::EndChild();

				ImGui::EndChild(); // Entity List
				ImGui::SameLine(); 
				//if (_cam != nullptr) {
				//	static char const *pipelineNames[RenderType::TOTAL] = { "Debug deferred rendering", "Deferred rendering" };
				//	ImGui::ListBox("Pipelines", &pipelineIndex, pipelineNames, int(RenderType::TOTAL));
				//	if (_cam->getPipeline() != (RenderType)pipelineIndex)
				//		_cam->setPipeline((RenderType)pipelineIndex);
				//}
			}

			bool EntityManager::initialize()
			{
				return true;
			}

			void EntityManager::generateBasicEntities()
			{
				auto camera = _scene->createEntity();
				_cam = camera.addComponent<CameraComponent>();
				camera.getLink().setPosition(glm::vec3(0, 3, 5));
				camera.getLink().setForward(glm::vec3(0, 0, 0));
				camera.addComponent<FreeFlyComponent>();
				camera.addComponent<AGE::WE::EntityRepresentation>()->editorOnly = true;
			}
	}
}