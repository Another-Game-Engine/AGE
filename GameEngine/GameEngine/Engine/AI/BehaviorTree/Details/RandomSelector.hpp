#pragma once

#include <chrono>
#include <random>

#include <AI/BehaviorTree/Details/CompositeNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class RandomSelector final : public CompositeNode < Entity, Args... >
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;

				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;

				using Parent = typename CompositeNode<Entity, Args...>::Parent;

				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;

				using Child = typename CompositeNode<Entity, Args...>::Child;

				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;

				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;

				// Attributes
				ChildPtr currentNode;

				// Inherited Methods
				void initialize(EntityPtr entity) override final
				{
					currentNode.reset();
					for (auto &child : this->children)
					{
						child->initialize(entity);
					}
				}

				Status execute(EntityPtr entity, Args... args) override final
				{
					if (currentNode)
					{
						Status const status = currentNode->execute(entity, args...);
						if (status != Status::Running)
						{
							currentNode.reset();
						}
						return status;
					}
					else
					{
						std::size_t const childrenNumber = this->children.size();
						if (childrenNumber != 0)
						{
							static std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
							std::uniform_int_distribution<std::size_t> distribution(0, childrenNumber - 1);
							std::size_t const chosen = distribution(generator);
							currentNode = this->children[chosen];
							Status const status = currentNode->execute(entity, args...);
							if (status != Status::Running)
							{
								currentNode.reset();
							}
							return status;
						}
						else
						{
							assert(!"No children assigned");
							return Status::Failure;
						}
					}
				}

			public:
				// Constructors
				RandomSelector(void) = default;

				RandomSelector(RandomSelector const &) = default;

				// Assignment Operators
				RandomSelector &operator=(RandomSelector const &) = default;

				// Destructor
				~RandomSelector(void) = default;
			};
		}
	}
}
