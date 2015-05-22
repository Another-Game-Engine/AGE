#pragma once

namespace AGE
{
	namespace Physics
	{
		class WorldInterface;

		class RaycasterInterface
		{
			// Friendships
			friend WorldInterface;

		public:
			// Constructors
			RaycasterInterface(void) = delete;

			RaycasterInterface(WorldInterface *world);

			RaycasterInterface(const RaycasterInterface &) = delete;

			// Assignment Operators
			RaycasterInterface &operator=(const RaycasterInterface &) = delete;

			// Methods
			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

		protected:
			// Attributes
			WorldInterface *world = nullptr;

			// Destructor
			virtual ~RaycasterInterface(void) = default;
		};
	}
}

#include "RaycasterInterface.inl"