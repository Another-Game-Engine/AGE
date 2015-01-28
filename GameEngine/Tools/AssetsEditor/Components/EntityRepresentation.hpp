#pragma once

#include <Components/Component.hh>
#include <array>

namespace AGE
{
	namespace WE
	{
		struct EntityRepresentation : public Component::ComponentBase < EntityRepresentation >
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
			
			
			//EntityRepresentation &operator=(EntityRepresentation const &o) = delete;
			//EntityRepresentation(EntityRepresentation const &o) = delete;
			EntityRepresentation &operator=(EntityRepresentation &&o);
			EntityRepresentation(EntityRepresentation &&o);
		};
	}
}