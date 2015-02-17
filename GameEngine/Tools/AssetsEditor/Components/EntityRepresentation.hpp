#pragma once

#include <Components/Component.hh>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>

#define ENTITY_NAME_LENGTH 128

namespace AGE
{
	namespace WE
	{
		struct EntityRepresentation : public ComponentBase
		{
			EntityRepresentation();

			virtual ~EntityRepresentation(void);

			void init(AScene *, const char* name = "NoName");

			virtual void reset(AScene *);

			template <typename Archive>
			void serialize(Archive &ar)
			{
				std::string sname = name;
				ar(cereal::make_nvp("name", sname));
				sname.resize(ENTITY_NAME_LENGTH - 1);
				memcpy(name, sname.data(), sname.length());
				name[ENTITY_NAME_LENGTH - 1] = 0;
			}

			virtual void postUnserialization(AScene *scene);

			char name[ENTITY_NAME_LENGTH];
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			
			//EntityRepresentation &operator=(EntityRepresentation const &o) = delete;
			//EntityRepresentation(EntityRepresentation const &o) = delete;
			EntityRepresentation &operator=(EntityRepresentation &&o);
			EntityRepresentation(EntityRepresentation &&o);
		};
	}
}