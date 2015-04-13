#pragma once

#include <AI/BehaviorTree/Details/DecoratorNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template <class Entity, typename... Args>
			class RunningAfterDecorator final : public DecoratorNode < Entity, Args... >
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
				std::size_t total;

				std::size_t count;

				// Inherited Methods
				void initialize(EntityPtr entity) override final
				{
					count = total;
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				Status filter(EntityPtr entity, Args... args) override final
				{
					if (count == 0)
					{
						if (!this->children.empty())
						{
							Status const status = this->children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								this->children.front()->initialize(entity);
							}
						}
						return Status::Running;
					}
					else
					{
						--count;
						if (!this->children.empty())
						{
							Status const status = this->children.front()->execute(entity, args...);
							if (status == Status::Success || status == Status::Failure)
							{
								this->children.front()->initialize(entity);
							}
							return status;
						}
						return Status::Running;
					}
				}

			public:
				// Constructors
				RunningAfterDecorator(void) = delete;

				RunningAfterDecorator(std::size_t total)
					: DecoratorNode<Entity, Args...>(), total(total), count(total)
				{
					return;
				}

				RunningAfterDecorator(RunningAfterDecorator const &) = default;

				// Assignment Operators
				RunningAfterDecorator &operator=(RunningAfterDecorator const &) = default;

				// Destructor
				~RunningAfterDecorator(void) = default;
			};
		}
	}
}
