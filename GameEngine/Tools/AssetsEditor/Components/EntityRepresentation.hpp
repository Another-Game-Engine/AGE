#pragma once

#include <Components/Component.hh>
#include <array>
#include <glm/fwd.hpp>

namespace AGE
{
	namespace WE
	{
		struct EntityRepresentation : public ComponentBase
		{
			EntityRepresentation();

			virtual ~EntityRepresentation(void);

			void init(AScene *, const char* name);

			virtual void reset(AScene *);

			template <typename Archive>
			void serialize(Archive &ar)
			{

			}

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