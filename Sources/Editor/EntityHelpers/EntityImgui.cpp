#include "EntityImgui.hpp"

#include <Entity/Entity.hh>
#include <Components/EntityRepresentation.hpp>
#include <Components/ComponentRegistrationManager.hpp>
#include <ComponentsCore/MeshRenderer.hh>

#include <Components/ArchetypeComponent.hpp>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace AGE
{
	namespace WE
	{
		void listEntityTree(Entity &entity, std::vector<const char*> &names, std::vector<Entity*> &entities)
		{
			if (!entity.isValid())
			{
				return;
			}
			if (!entity->haveComponent<EntityRepresentation>())
			{
				return;
			}

			auto er = entity->getComponent<EntityRepresentation>();
			names.push_back(er->name);
			entities.push_back(entity.getPtr());

			for (auto &c : entity->getLink().getChildren())
			{
				listEntityTree(c->getEntity()->getEntity(), names, entities);
			}
		}


		//return true if entity has been modified
		bool displayEntity(Entity &entity, AScene *scene)
		{
			bool modified = false;

			auto cpt = entity->getComponent<AGE::WE::EntityRepresentation>();
			auto archetypeCpt = entity->getComponent<AGE::ArchetypeComponent>();


			ImGui::InputText("Name", cpt->name, ENTITY_NAME_LENGTH);

			auto meshRenderer = entity->getComponent<MeshRenderer>();
			cpt->position = entity->getLink().getPosition();

			bool updatePos = true;
			bool updateRot = true;
			bool updateSca = true;

			if (archetypeCpt)
			{
				updatePos = !archetypeCpt->synchronizePosition;
				updateRot = !archetypeCpt->synchronizeRotation;
				updateSca = !archetypeCpt->synchronizeScale;
			}

			// if entity have a mesh renderer component
			// we pad the position so the origin is not in the center of the geometry
			// but in 0,0,0
			//if (meshRenderer && meshRenderer->getMesh())
			//{
			//	auto mesh = meshRenderer->getMesh();
			//	cpt->position += mesh->meshData->boundingBox.getSize() / 2.0f;
			//}
			if (updatePos && ImGui::InputFloat3("Position", glm::value_ptr(cpt->position)))
			{
				modified = true;
				//if (meshRenderer && meshRenderer->getMesh())
				//{
				//	auto mesh = meshRenderer->getMesh();
				//	cpt->position -= mesh->meshData->boundingBox.getSize() / 2.0f;
				//}
				entity->getLink().setPosition(cpt->position);
			}

			if (updatePos && cpt->position != entity->getLink().getPosition())
			{
				entity->getLink().setPosition(cpt->position);
			}

			if (updateRot && ImGui::InputFloat3("Rotation", glm::value_ptr(cpt->rotation)))
			{
				modified = true;
				entity->getLink().setOrientation(glm::quat(cpt->rotation * glm::pi<float>() / 180.f));
			}

			if (updateRot &&  glm::quat(cpt->rotation * glm::pi<float>() / 180.f) != entity->getLink().getOrientation())
			{
				entity->getLink().setOrientation(glm::quat(cpt->rotation * glm::pi<float>() / 180.f));
			}

			if (updateSca && ImGui::InputFloat3("Scale", glm::value_ptr(cpt->scale)))
			{
				modified = true;
				entity->getLink().setScale(cpt->scale);
			}

			if (updateSca && cpt->scale != entity->getLink().getScale())
			{
				entity->getLink().setScale(cpt->scale);
			}

			ImGui::Separator();

			if (archetypeCpt && archetypeCpt->parentIsAnArchetype == false)
			{
				ImGui::TextColored(ImVec4(0.8f, 0.4f, 0.5f, 1.0f), "Entity is Archetype, edit the proper archetype.");

				auto archetypeComponent = entity->getComponent<AGE::ArchetypeComponent>();
				modified |= archetypeComponent->editorUpdate();

				return modified;
			}
			else if (archetypeCpt && archetypeCpt->parentIsAnArchetype == true)
			{
				ImGui::TextColored(ImVec4(0.8f, 0.4f, 0.5f, 1.0f), "Entity's parent is Archetype, edit the proper archetype.");

				auto archetypeComponent = entity->getComponent<AGE::ArchetypeComponent>();
				modified |= archetypeComponent->editorUpdate();

				return modified;
			}

			auto &components = entity->getComponentList();
			for (ComponentType i = 0; i < components.size(); ++i)
			{
				if (entity->haveComponent(i))
				{
					auto ptr = entity->getComponent(i);
					if (ptr->isExposedInEditor())
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
								modified = true;
								ptr->editorDelete();
								entity->removeComponent(i);
							}
							ImGui::PopID();
						}
						if (opened)
						{
							if (!deleted && ptr->editorUpdate())
							{
								modified = true;
							}
							ImGui::TreePop();
						}
					}
				}
			}
			return modified;
		}

		//return true if entity has been modified
		bool recursiveDisplayList(Entity &entity, Entity *& selectedEntity, bool &selectParent, bool editableHierarchy)
		{
			bool modified = false;

			auto cpt = entity->getComponent<AGE::WE::EntityRepresentation>();
			auto archetypeComponent = entity->getComponent<AGE::ArchetypeComponent>();

			bool opened = false;
			opened = ImGui::TreeNode(cpt->name);
			ImGui::PushID(entity.getPtr());
			if (selectedEntity != entity.getPtr())
			{
				if (!selectParent)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Select"))
					{
						selectedEntity = entity.getPtr();
					}
				}
				else if (editableHierarchy && archetypeComponent == nullptr)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Set as parent"))
					{
						modified = true;
						(*selectedEntity)->getLink().attachParent(entity.getLinkPtr());
						selectParent = false;
					}
				}
			}
			else
			{
				auto canChangeHierarchy = archetypeComponent == nullptr || (archetypeComponent->parentIsAnArchetype == false);
				if (!selectParent && editableHierarchy && canChangeHierarchy)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Set parent"))
					{
						selectParent = true;
					}
				}
				else if (editableHierarchy && canChangeHierarchy)
				{
					ImGui::SameLine();
					if (ImGui::SmallButton("Root"))
					{
						modified = true;
						(*selectedEntity)->getLink().detachParent();
						selectParent = false;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("Cancel"))
					{
						selectParent = false;
					}
				}
			}
			ImGui::PopID();
			if (opened)
			{
				if (entity->getLink().hasChildren())
				{
					auto children = entity->getLink().getChildren();
					for (auto &e : children)
					{
						if (recursiveDisplayList(e->getEntity()->getEntity(), selectedEntity, selectParent, editableHierarchy))
						{
							modified = true;
						}
					}
				}
				ImGui::TreePop();
			}
			return modified;
		}
	}
}