#pragma once

#include <AI/BehaviorTree/Details/CompositeNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class DecoratorNode : public CompositeNode < Entity, Args... >
			{
			protected:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;

				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;

				using Parent = typename CompositeNode<Entity, Args...>::Parent;

				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;

				using Child = typename CompositeNode<Entity, Args...>::Child;

				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;

				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;

			public:
				// Destructor
				virtual ~DecoratorNode(void) = default;

				// Inherited Methods
				ParentPtr addChild(ChildPtr child) override final
				{
					assert(this->children.empty() && "Tree should be empty");
					return CompositeNode<Entity, Args...>::addChild(child);
				}

			protected:
				// Constructors
				DecoratorNode(void) = default;

				DecoratorNode(DecoratorNode const &) = default;

				// Assignment Operators
				DecoratorNode &operator=(DecoratorNode const &) = default;

				// Virtual Methods
				virtual void initialize(EntityPtr) override = 0;

				virtual Status filter(EntityPtr entity, Args... args) = 0;

				// Inherited Methods
				Status execute(EntityPtr entity, Args... args) override final
				{
					return filter(entity, args...);
				}
			};
		}
	}
}
