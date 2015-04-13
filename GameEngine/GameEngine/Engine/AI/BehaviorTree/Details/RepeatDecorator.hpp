#pragma once

#include <cstddef>

#include <AI/BehaviorTree/Details/DecoratorNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class RepeatDecorator final : public DecoratorNode < Entity, Args... >
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
				std::size_t target;

				std::size_t count = 0;

				// Inherited Methods
				void initialize(EntityPtr entity) override final
				{
					count = 0;
					if (!this->children.empty())
					{
						this->children.front()->initialize(entity);
					}
				}

				Status filter(EntityPtr entity, Args... args) override final
				{
					if (this->children.empty())
					{
						return Status::Success;
					}
					else
					{
						Status const status = this->children.front()->execute(entity, args...);
						if (status == Status::Success)
						{
							++count;
							if (count == target)
							{
								initialize(entity);
								return Status::Success;
							}
							else
							{
								return Status::Running;
							}
						}
						else
						{
							return status;
						}
					}
				}

			public:
				// Constructors
				RepeatDecorator(void) = delete;

				RepeatDecorator(std::size_t repeats)
					: DecoratorNode<Entity, Args...>(), target(repeats)
				{
					return;
				}

				RepeatDecorator(RepeatDecorator const &) = default;

				// Assignment Operators
				RepeatDecorator &operator=(RepeatDecorator const &) = default;

				// Destructor
				~RepeatDecorator(void) = default;
			};
		}
	}
}
