#pragma once

#include <functional>
#include <limits>
#include <cmath>
#include <cassert>

#include <AI/BehaviorTree/Details/ConditionNode.hpp>
#include <AI/BehaviorTree/Details/ConditionTest.hpp>
#include <AI/BehaviorTree/Details/Private/DecimalConditionEnabler.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < typename DecimalType, class Entity, typename... Args >
			class DecimalCondition final : public ConditionNode<Entity, Args...>, public Private::DecimalConditionEnabler < DecimalType >
			{
			private:
				using EntityType = typename ConditionNode<Entity, Args...>::EntityType;

				using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;

				using Parent = typename ConditionNode<Entity, Args...>::Parent;

				using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;

				using Function = std::function < DecimalType(EntityPtr, Args...) > ;

				// Attributes
				Function function;

				ConditionTest condition;

				DecimalType target;

				DecimalType epsilon;

				// Static Methods
				static bool Equals(DecimalType lhs, DecimalType rhs, DecimalType epsilon)
				{
					return std::abs(lhs - rhs) <= epsilon;
				}

				// Inherited Methods
				void initialize(EntityPtr) override final
				{
					return;
				}

				Status decide(EntityPtr entity, Args... args) override final
				{
					DecimalType const result = function(entity, args...);
					switch (condition)
					{
						case ConditionTest::LessThan:
							return result < target && !DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
						case ConditionTest::GreaterThan:
							return result > target && !DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
						case ConditionTest::LessOrEqualThan:
							return result < target || DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
						case ConditionTest::GreaterOrEqualThan:
							return result > target || DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
						case ConditionTest::Equal:
							return DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
						case ConditionTest::NotEqual:
							return !DecimalCondition::Equals(result, target, epsilon) ? Status::Success : Status::Failure;
						default:
							return Status::Failure;
					}
				}

			public:
				// Constructors
				DecimalCondition(void) = delete;

				DecimalCondition(Function const &function, ConditionTest condition, DecimalType target, DecimalType epsilon = std::numeric_limits<DecimalType>::epsilon())
					: ConditionNode<Entity, Args...>(), Private::DecimalConditionEnabler<DecimalType>(), function(function), condition(condition), target(target), epsilon(epsilon)
				{
					assert(function && "Invalid function");
				}

				DecimalCondition(DecimalCondition const &) = default;

				// Assignment Operators
				DecimalCondition &operator=(DecimalCondition const &) = default;

				// Destructor
				~DecimalCondition(void) = default;
			};
		}
	}
}
