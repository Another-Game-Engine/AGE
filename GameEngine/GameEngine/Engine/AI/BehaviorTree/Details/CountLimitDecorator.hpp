#pragma once

#include <AI/BehaviorTree/Details/DecoratorNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class CountLimitDecorator final : public DecoratorNode < Entity, Args... >
			{
			private:
				using EntityType = typename DecoratorNode<Entity, Args...>::EntityType;

				using EntityPtr = typename DecoratorNode<Entity, Args...>::EntityPtr;

				using Parent = typename DecoratorNode<Entity, Args...>::Parent;

				using ParentPtr = typename DecoratorNode<Entity, Args...>::ParentPtr;

				using Child = typename DecoratorNode<Entity, Args...>::Child;

				using ChildPtr = typename DecoratorNode<Entity, Args...>::ChildPtr;

				using ChildrenList = typename DecoratorNode<Entity, Args...>::ChildrenList;

				// Attributes
				std::size_t limit;

				std::size_t currentRepetition = 0;

				bool allowReinitialize;

				// Inherited Methods
				void initialize(EntityPtr entity) override final
				{
					if (allowReinitialize)
					{
						currentRepetition = 0;
					}
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				Status filter(EntityPtr entity, Args... args) override final
				{
					if (currentRepetition == limit)
					{
						return Status::Failure;
					}
					else if (this->children.empty())
					{
						++currentRepetition;
						return Status::Success;
					}
					else
					{
						Status const status = this->children.front()->execute(entity, args...);
						if (status == Status::Success || status == Status::Failure)
						{
							++currentRepetition;
							if (!this->children.empty())
							{
								this->children.front()->initialize(entity);
							}
						}
						return status;
					}
				}

			public:
				// Constructors
				CountLimitDecorator(void) = delete;

				CountLimitDecorator(std::size_t limit, bool allowReinitialize = true)
					: DecoratorNode<Entity, Args...>(), limit(limit), allowReinitialize(allowReinitialize)
				{
					return;
				}

				CountLimitDecorator(CountLimitDecorator const &) = default;

				// Assignment Operators
				CountLimitDecorator &operator=(CountLimitDecorator const &) = default;

				// Destructor
				~CountLimitDecorator(void) = default;
			};
		}
	}
}
