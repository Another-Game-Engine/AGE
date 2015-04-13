#pragma once

#include <AI/BehaviorTree/Details/DecoratorNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class AlwaysSuccessDecorator final : public DecoratorNode < Entity, Args... >
			{
			private:
				using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;

				using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;

				using Parent = typename DecoratorNode<Entity, Args...>::Parent;

				using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;

				using Child = typename DecoratorNode<Entity, Args...>::Child;

				using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;

				using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

				// Inherited Methods
				void initialize(EntityPtr entity) override final
				{
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				Status filter(EntityPtr entity, Args... args) override final
				{
					if (!this->children.empty())
					{
						Status const status = this->children.front()->execute(entity, args...);
						if (status == Status::Success || status == Status::Failure)
						{
							this->children.front()->initialize(entity);
						}
					}
					return Status::Success;
				}

			public:
				// Constructors
				AlwaysSuccessDecorator(void) = default;

				AlwaysSuccessDecorator(AlwaysSuccessDecorator const &) = default;

				// Assignment Operators
				AlwaysSuccessDecorator &operator=(AlwaysSuccessDecorator const &) = default;

				// Destructor
				~AlwaysSuccessDecorator(void) = default;
			};
		}
	}
}
