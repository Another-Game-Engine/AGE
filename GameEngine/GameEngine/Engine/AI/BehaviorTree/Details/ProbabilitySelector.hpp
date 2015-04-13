#pragma once

#include <unordered_map>
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
			class ProbabilitySelector final : public CompositeNode < Entity, Args... >
			{
			private:
				using EntityType = typename CompositeNode<Entity, Args...>::EntityType;

				using EntityPtr = typename CompositeNode<Entity, Args...>::EntityPtr;

				using Parent = typename CompositeNode<Entity, Args...>::Parent;

				using ParentPtr = typename CompositeNode<Entity, Args...>::ParentPtr;

				using Child = typename CompositeNode<Entity, Args...>::Child;

				using ChildPtr = typename CompositeNode<Entity, Args...>::ChildPtr;

				using ChildrenList = typename CompositeNode<Entity, Args...>::ChildrenList;

				using WeightingMap = std::unordered_map < ChildPtr, float > ;

				// Attributes
				float totalSum = 0.0f;

				ChildPtr currentNode;

				WeightingMap weightingMap;

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
						static std::default_random_engine generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
						std::uniform_real_distribution<float> distribution(0.0f, totalSum);
						float const chosen = distribution(generator);
						float sum = 0.0f;
						for (auto &weighting : weightingMap)
						{
							sum += weighting.second;
							if (sum >= chosen)
							{
								Status const status = weighting.first->execute(entity, args...);
								if (status == Status::Running)
								{
									currentNode = weighting.first;
								}
								return status;
							}
						}
						assert(!"Weighting is too high");
						return Status::Failure;
					}
				}

			public:
				// Constructors
				ProbabilitySelector(void) = default;

				ProbabilitySelector(ProbabilitySelector const &) = default;

				// Assignment Operators
				ProbabilitySelector &operator=(ProbabilitySelector const &) = default;

				// Destructor
				~ProbabilitySelector(void) = default;

				// Inherited Methods
				ParentPtr addChild(ChildPtr child) override final
				{
					return addChild(child, 1.0f);
				}

				// Methods
				ParentPtr addChild(ChildPtr child, float weighting)
				{
					assert(child && "Invalid child");
					assert(weighting >= 0.0f && "Invalid weighting");
					weightingMap[child] = weighting;
					totalSum += weighting;
					return CompositeNode<Entity, Args...>::addChild(child);
				}
			};
		}
	}
}