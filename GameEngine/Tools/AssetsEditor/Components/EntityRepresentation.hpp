#pragma once

#include <Components/Component.hh>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	namespace WE
	{
		struct ArchetypeEditorRepresentation;

		struct EntityRepresentation : public ComponentBase
		{
			AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_EntityRepresentationComponent");

			EntityRepresentation();

			virtual ~EntityRepresentation(void);

			void init(const char* name = "NoName", const std::string &layerName = "");

			virtual void reset();

			virtual void _copyFrom(const ComponentBase *model);

			template <typename Archive>
			void serialize(Archive &ar, const std::uint32_t version)
			{
				std::string sname = name;
				ar(cereal::make_nvp("name", sname));
				sname.resize(ENTITY_NAME_LENGTH - 1);
				memcpy(name, sname.data(), sname.length());
				name[ENTITY_NAME_LENGTH - 1] = 0;
			}

			virtual void postUnserialization();

			inline bool isLinkedToArchetype() const
			{
				return _archetypeLinked != nullptr;
			}

			inline bool isArchetype() const
			{
				return _isArchetype;
			}

			inline bool parentIsArchetype() const
			{
				return _parentIsArchetype;
			}

			char name[ENTITY_NAME_LENGTH];
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			//if it's a direct instance of an archetype
			std::shared_ptr<ArchetypeEditorRepresentation> _archetypeLinked;

			//if it's an archetype
			bool _isArchetype = false;

			//if is the child of an instance of an archetype
			bool _parentIsArchetype = false;


			// component is not serialized in export
			virtual bool serializeInExport() { return false; }
			virtual bool isExposedInEditor(){ return false; }
		
			// entity is not displayed in entity list
			bool editorOnly = false;
		};
	}
}

CEREAL_CLASS_VERSION(AGE::WE::EntityRepresentation, 0)