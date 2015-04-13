#pragma once

#include <memory>

#include <AI/BehaviorTree/Details/RootNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class CompositeNode;

			template <class Entity, typename... Args>
			class LeafNode : public RootNode < Entity, Args... >
			{
				friend class CompositeNode < Entity, Args... > ;

			protected:
				using EntityType = typename RootNode<Entity, Args... >::EntityType;

				using EntityPtr = typename RootNode<Entity, Args... >::EntityPtr;

				using Parent = CompositeNode < Entity, Args... > ;

				using ParentPtr = std::shared_ptr < Parent > ;

				// Constructors
				LeafNode(void) = default;

				LeafNode(LeafNode const &) = default;

				// Assignment Operators
				LeafNode &operator=(LeafNode const &) = default;

				// Virtual Methods
				virtual void onParentAdded(ParentPtr)
				{
					return;
				}

				virtual void onParentRemoved(ParentPtr)
				{
					return;
				}

			public:
				// Destructor
				virtual ~LeafNode(void) = default;

				// Virtual Methods
				virtual void initialize(EntityPtr entity) override = 0;

				virtual Status execute(EntityPtr entity, Args... args) override = 0;
			};
		}
	}
}