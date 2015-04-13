#pragma once

#include <functional>
#include <cassert>

#include <AI/BehaviorTree/Details/ConditionNode.hpp>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			template < class Entity, typename... Args >
			class BooleanCondition final : public ConditionNode < Entity, Args... >
			{
			private:
				using EntityType = typename ConditionNode<Entity, Args...>::EntityType;

				using EntityPtr = typename ConditionNode<Entity, Args...>::EntityPtr;

				using Parent = typename ConditionNode<Entity, Args...>::Parent;

				using ParentPtr = typename ConditionNode<Entity, Args...>::ParentPtr;

				using Function = std::function < bool(EntityPtr, Args...) > ;

				// Attributes
				Function function;

				bool testValue;

				// Inherited Methods
				void initialize(EntityPtr) override final
				{
					return;
				}

				Status decide(EntityPtr entity, Args... args) override final
				{
					return function(entity, args...) == testValue ? Status::Success : Status::Failure;
				}

			public:
				// Constructors
				BooleanCondition(void) = delete;

				BooleanCondition(Function const &function, bool testValue = true)
					: ConditionNode<Entity, Args...>(), function(function), testValue(testValue)
				{
					assert(function && "Invalid function");
				}

				BooleanCondition(BooleanCondition const &) = default;

				// Assignment Operators
				BooleanCondition &operator=(BooleanCondition const &) = default;

				// Destructor
				~BooleanCondition(void) = default;
			};
		}
	}
}
