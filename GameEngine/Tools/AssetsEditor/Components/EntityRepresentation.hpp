#pragma once

#include <Components/Component.hh>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <Core/AScene.hh>

#define ENTITY_NAME_LENGTH 128

namespace AGE
{
	namespace WE
	{
		struct EntityRepresentation : public ComponentBase
		{
			EntityRepresentation();

			virtual ~EntityRepresentation(void);

			void init(const char* name = "NoName", const std::string &layerName = "");

			virtual void reset();

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

			char name[ENTITY_NAME_LENGTH];
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			// component is not serialized in export
#ifdef EDITOR_ENABLED
			virtual bool serializeInExport() { return false; }
#endif
			// entity is not displayed in entity list
			bool editorOnly = false;
		};
	}
}

CEREAL_CLASS_VERSION(AGE::WE::EntityRepresentation, 0)