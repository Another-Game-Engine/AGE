#pragma once

#include <AI/BehaviorTree/Details/CompositeNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class SequenceSelector final : public CompositeNode < Entity, Args... >
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
					if (currentPosition == InitialPosition)
					{
						initialize(entity);
						currentPosition = 0;
					}
					if (this->children.empty())
					{
						return Status::Success;
					}
					ChildPtr currentTask = this->children[currentPosition];
					Status result = currentTask->execute(entity, args...);
					std::size_t const lastPosition = this->children.size() - 1;
					while (result == Status::Success)
					{
						if (currentPosition == lastPosition)
						{
							currentPosition = InitialPosition;
							return Status::Success;
						}
						else
						{
							++currentPosition;
							currentTask = this->children[currentPosition];
							result = currentTask->execute(entity, args...);
						}
					}
					if (result == Status::Failure)
					{
						currentPosition = InitialPosition;
					}
					return result;
				}

			public:
				// Constructors
				SequenceSelector(void) = default;

				SequenceSelector(SequenceSelector const &) = default;

				// Assignment Operators
				SequenceSelector &operator=(SequenceSelector const &) = default;

				// Destructor
				~SequenceSelector(void) = default;
			};
		}
	}
}
