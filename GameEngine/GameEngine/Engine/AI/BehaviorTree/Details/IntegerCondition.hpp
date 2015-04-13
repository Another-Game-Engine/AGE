#pragma once

#include <functional>
#include <cassert>

#include <AI/BehaviorTree/Details/ConditionNode.hpp>
#include <AI/BehaviorTree/Details/ConditionTest.hpp>
#include <AI/BehaviorTree/Details/Private/IntegerConditionEnabler.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < typename IntegerType, class Entity, typename... Args >
			class IntegerCondition final : public ConditionNode<Entity, Args...>, public Private::IntegerConditionEnabler < IntegerType >
			{
			private:
				using EntityType = typename ConditionNode<Entity, Args...>::EntityType;

				using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;

				using Parent = typename ConditionNode<Entity, Args...>::Parent;

				using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;

				using Function = std::function < IntegerType(EntityPtr, Args...) > ;

				// Attributes
				Function function;

				ConditionTest condition;

				IntegerType target;

				// Inherited Methods
				void initialize(EntityPtr) override final
				{
					return;
				}

				Status decide(EntityPtr entity, Args...args) override final
				{
					IntegerType const result = function(entity, args...);
					switch (condition)
					{
						case ConditionTest::LessThan:
							return result < target ? Status::Success : Status::Failure;
						case ConditionTest::GreaterThan:
							return result > target ? Status::Success : Status::Failure;
						case ConditionTest::LessOrEqualThan:
							return result <= target ? Status::Success : Status::Failure;
						case ConditionTest::GreaterOrEqualThan:
							return result >= target ? Status::Success : Status::Failure;
						case ConditionTest::Equal:
							return result == target ? Status::Success : Status::Failure;
						case ConditionTest::NotEqual:
							return result != target ? Status::Success : Status::Failure;
						default:
							return Status::Failure;
					}
				}

			public:
				// Constructors
				IntegerCondition(void) = delete;

				IntegerCondition(Function const &function, ConditionTest condition, IntegerType target)
					: ConditionNode<Entity, Args...>(), Private::IntegerConditionEnabler<IntegerType>(), function(function), condition(condition), target(target)
				{
					assert(function && "Invalid function");
				}

				IntegerCondition(IntegerCondition const &) = default;

				// Assignment Operators
				IntegerCondition &operator=(IntegerCondition const &) = default;

				// Destructor
				~IntegerCondition(void) = default;
			};
		}
	}
}
