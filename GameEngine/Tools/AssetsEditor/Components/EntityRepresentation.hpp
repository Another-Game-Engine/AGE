#pragma once

#include <Components/Component.hh>

namespace AGE
{
	namespace WE
	{
		struct EntityRepresentation : public Component::ComponentBase < EntityRepresentation >
		{
			EntityRepresentation()
			{

			}

			virtual ~EntityRepresentation(void)
			{

			}

			EntityRepresentation(EntityRepresentation &&o)
				: ComponentBase<EntityRepresentation>(std::move(o))
			{
				_t = std::move(o._t);
			}

			EntityRepresentation &operator=(EntityRepresentation &&o)
			{
				_t = std::move(o._t);
				return *this;
			}

			void init(AScene *, float t)
			{
				_t = t;
			}

			virtual void reset(AScene *)
			{
				_t = 0.0f;
			}

			//////
			////
			// Serialization

			template <typename Archive>
			void serialize(Archive &ar)
			{
				ar(CEREAL_NVP(_t));
			}

			// !Serialization
			////
			//////

			float _t;
		private:
			EntityRepresentation &operator=(EntityRepresentation const &o);
			EntityRepresentation(EntityRepresentation const &o);
		};
	}
}