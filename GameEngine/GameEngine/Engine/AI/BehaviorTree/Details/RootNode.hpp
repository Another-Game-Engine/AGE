#pragma once

#include <AI/BehaviorTree/Details/Status.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class RootNode
			{
			protected:
				using EntityType = Entity;

				using EntityPtr = EntityType *;

				// Constructors
				RootNode(void) = default;

				RootNode(RootNode const &) = default;

				// Assignment Operators
				RootNode &operator=(RootNode const &) = default;

			public:
				// Destructor
				virtual ~RootNode(void) = default;

				// Virtual Methods
				virtual void initialize(EntityPtr entity) = 0;

				virtual Status execute(EntityPtr entity, Args... args) = 0;
			};
		}
	}
}