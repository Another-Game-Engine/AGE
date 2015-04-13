#pragma once

// Status
#include <AI/BehaviorTree/Details/Status.hpp>

// Policies
#include <AI/BehaviorTree/Details/FailurePolicy.hpp>
#include <AI/BehaviorTree/Details/SuccessPolicy.hpp>

// Conditions
#include <AI/BehaviorTree/Details/ConditionTest.hpp>

// Root Nodes
#include <AI/BehaviorTree/Details/RootNode.hpp>

// Base Nodes
#include <AI/BehaviorTree/Details/ActionNode.hpp>
#include <AI/BehaviorTree/Details/CompositeNode.hpp>
#include <AI/BehaviorTree/Details/ConditionNode.hpp>
#include <AI/BehaviorTree/Details/DecoratorNode.hpp>

// Condition Nodes
#include <AI/BehaviorTree/Details/FunctionCondition.hpp>
#include <AI/BehaviorTree/Details/BooleanCondition.hpp>
#include <AI/BehaviorTree/Details/DecimalCondition.hpp>
#include <AI/BehaviorTree/Details/IntegerCondition.hpp>

// Decorator Nodes
#include <AI/BehaviorTree/Details/AlwaysFailureDecorator.hpp>
#include <AI/BehaviorTree/Details/AlwaysRunningDecorator.hpp>
#include <AI/BehaviorTree/Details/AlwaysSuccessDecorator.hpp>
#include <AI/BehaviorTree/Details/FailureAfterDecorator.hpp>
#include <AI/BehaviorTree/Details/RunningAfterDecorator.hpp>
#include <AI/BehaviorTree/Details/SuccessAfterDecorator.hpp>
#include <AI/BehaviorTree/Details/CountLimitDecorator.hpp>
#include <AI/BehaviorTree/Details/RepeatDecorator.hpp>

// Selector Nodes
#include <AI/BehaviorTree/Details/ConcurrentSelector.hpp>
#include <AI/BehaviorTree/Details/PrioritySelector.hpp>
#include <AI/BehaviorTree/Details/ProbabilitySelector.hpp>
#include <AI/BehaviorTree/Details/RandomSelector.hpp>
#include <AI/BehaviorTree/Details/SequenceSelector.hpp>

namespace AGE
{
	namespace AI
	{
		// Status
		using Status = Details::Status;

		// Policies
		using FailurePolicy = Details::FailurePolicy;

		using SuccessPolicy = Details::SuccessPolicy;

		// Conditions
		using ConditionTest = Details::ConditionTest;

		template <class Entity, typename... Args>
		class BehaviorTree
		{
		private:
			// Root Nodes
			using Root = typename Details::RootNode < Entity, Args... > ;

			using RootPtr = std::shared_ptr < Root > ;

		public:
			// Entity Type
			using EntityType = Entity;

			using EntityPtr = EntityType *;

			// Status
			using Status = Details::Status;

			// Policies
			using FailurePolicy = Details::FailurePolicy;

			using SuccessPolicy = Details::SuccessPolicy;

			// Conditions
			using ConditionTest = Details::ConditionTest;

			// Base Nodes
			using Action = typename Details::ActionNode < Entity, Args... > ;

			using Composite = typename Details::CompositeNode < Entity, Args... > ;

			using Condition = typename Details::ConditionNode < Entity, Args... > ;

			using Decorator = typename Details::DecoratorNode < Entity, Args... > ;

			// Condition Nodes
			using FunctionCondition = typename Details::FunctionCondition < Entity, Args... > ;

			using BooleanCondition = typename Details::BooleanCondition < Entity, Args... > ;

			template <typename DecimalType>
			using DecimalCondition = typename Details::DecimalCondition < DecimalType, Entity, Args... > ;

			template <typename IntegerType>
			using IntegerCondition = typename Details::IntegerCondition < IntegerType, Entity, Args... > ;

			// Decorator Nodes
			using AlwaysFailureDecorator = typename Details::AlwaysFailureDecorator < Entity, Args... > ;

			using AlwaysRunningDecorator = typename Details::AlwaysRunningDecorator < Entity, Args... > ;

			using AlwaysSuccessDecorator = typename Details::AlwaysSuccessDecorator < Entity, Args... > ;

			using FailureAfterDecorator = typename Details::FailureAfterDecorator < Entity, Args... > ;

			using RunningAfterDecorator = typename Details::RunningAfterDecorator < Entity, Args... > ;

			using SuccessAfterDecorator = typename Details::SuccessAfterDecorator < Entity, Args... > ;

			using CountLimitDecorator = typename Details::CountLimitDecorator < Entity, Args... > ;

			using RepeatDecorator = typename Details::RepeatDecorator < Entity, Args... > ;

			// Selector Nodes
			using ConcurrentSelector = typename Details::ConcurrentSelector < Entity, Args... > ;

			using PrioritySelector = typename Details::PrioritySelector < Entity, Args... > ;

			using ProbabilitySelector = typename Details::ProbabilitySelector < Entity, Args... > ;

			using RandomSelector = typename Details::RandomSelector < Entity, Args... > ;

			using SequenceSelector = typename Details::SequenceSelector < Entity, Args... > ;

			// Constructors
			BehaviorTree(void) = delete;

			BehaviorTree(EntityPtr owner)
				: owner(owner)
			{
				assert(owner && "Invalid entity");
				assignBehavior<AlwaysRunningDecorator>();
			}

			BehaviorTree(BehaviorTree const &) = default;

			// Assignment Operators
			BehaviorTree &operator=(BehaviorTree const &) = default;

			// Destructor
			virtual ~BehaviorTree(void) = default;

			// Methods
			Status update(Args... args)
			{
				return behavior->execute(owner, args...);
			}

			template <typename B, typename... BehaviorArgs>
			void assignBehavior(BehaviorArgs &&...args)
			{
				assignBehavior(std::make_shared<B>(std::forward<BehaviorArgs>(args)...));
			}

			void assignBehavior(RootPtr newBehavior)
			{
				assert(newBehavior && "Invalid behavior");
				behavior = newBehavior;
				reinitialize();
			}

			void reinitialize(void)
			{
				behavior->initialize(owner);
			}

		private:
			// Attributes
			EntityPtr owner;

			RootPtr behavior;
		};
	}
}
