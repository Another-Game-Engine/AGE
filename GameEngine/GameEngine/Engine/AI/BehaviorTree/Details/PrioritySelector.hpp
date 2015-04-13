#pragma once

#include <AI/BehaviorTree/Details/CompositeNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class PrioritySelector final : public CompositeNode < Entity, Args... >
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;

				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;

				using Parent = typename CompositeNode<Entity, Args...>::Parent;

				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;

				using Child = typename CompositeNode<Entity, Args...>::Child;

				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;

				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;

				// Static Attributes
				static const std::size_t InitialPosition = -1;

				// Attributes
				std::size_t currentPosition = InitialPosition;

				// Inherited Methods
				void initialize(EntityPtr entity) override final
				{
					currentPosition = InitialPosition;
					for (auto &child : this->children)
					{
						child->initialize(entity);
					}
				}

				Status execute(EntityPtr entity, Args... args) override final
				{
					if (currentPosition != InitialPosition)
					{
						Status const status = this->children[currentPosition]->execute(entity, args...);
						switch (status)
						{
							case Status::Running:
								return Status::Running;
							case Status::Success:
								currentPosition = InitialPosition;
								return Status::Success;
							case Status::Failure:
								++currentPosition;
								if (currentPosition == this->children.size())
								{
									currentPosition = InitialPosition;
									return Status::Failure;
								}
								else
								{
									break;
								}
							default:
								break;
						}
					}
					else
					{
						initialize(entity);
						currentPosition = 0;
					}
					if (this->children.empty())
					{
						return Status::Success;
					}
					ChildPtr currentlyRunningNode = this->children[currentPosition];
					Status status;
					while ((status = currentlyRunningNode->execute(entity, args...)) == Status::Failure)
					{
						++currentPosition;
						if (currentPosition == this->children.size())
						{
							currentPosition = InitialPosition;
							return Status::Failure;
						}
						currentlyRunningNode = this->children[currentPosition];
					}
					return status;
				}

			public:
				// Constructors
				PrioritySelector(void) = default;

				PrioritySelector(PrioritySelector const &) = default;

				// Assignment Operators
				PrioritySelector &operator=(PrioritySelector const &) = default;

				// Destructor
				~PrioritySelector(void) = default;
			};
		}
	}
}
