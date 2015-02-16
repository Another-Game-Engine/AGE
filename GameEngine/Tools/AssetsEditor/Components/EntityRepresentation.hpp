#pragma once

#include <Components/Component.hh>
#include <array>
#include <glm/fwd.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>

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
			//	ar(name);
			}

			virtual void postUnserialization(AScene *scene);

			std::array<char, 255> name;
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