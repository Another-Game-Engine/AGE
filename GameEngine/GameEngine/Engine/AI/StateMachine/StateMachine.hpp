#pragma once

#include <memory>
#include <stack>
#include <cassert>

#include <AI/StateMachine/State.hpp>
#include <AI/StateMachine/Private/EmptyState.hpp>

namespace AGE
{
	namespace AI
	{
		template <class Entity, typename... Args>
		class StateMachine
		{
		public:
			using EntityType = Entity;

			using EntityPtr = EntityType *;

			using ConstEntityPtr = EntityPtr const;

			using StateType = State < EntityType, Args... > ;

			using StatePtr = std::shared_ptr < StateType > ;

			using ConstStatePtr = std::shared_ptr < StateType const > ;

			using EmptyState = Private::EmptyState < Entity, Args... > ;

		private:
			// Attributes
			EntityPtr owner;

			std::stack<StatePtr> states;

			std::stack<StatePtr> globalStates;

		public:
			// Constructors
			StateMachine(void) = delete;

			StateMachine(EntityPtr owner)
				: owner(owner), states(), globalStates()
			{
				assert(owner && "Invalid owner");
				states.push(EmptyState::GetInstance());
				globalStates.push(EmptyState::GetInstance());
			}

			StateMachine(StateMachine const &) = delete;

			// Assignment operators
			StateMachine &operator=(StateMachine const &) = delete;

			// Destructor
			virtual ~StateMachine(void) = default;

			// Methods
			EntityPtr getOwner(void)
			{
				return owner;
			}

			ConstEntityPtr getOwner(void) const
			{
				return owner;
			}

			StatePtr getCurrentState(void)
			{
				assert(!states.empty() && "Stack underflow");
				return states.top();
			}

			ConstStatePtr getCurrentState(void) const
			{
				assert(!states.empty() && "Stack underflow");
				return states.top();
			}

			StatePtr getGlobalState(void)
			{
				assert(!globalStates.empty() && "Stack underflow");
				return globalStates.top();
			}

			ConstStatePtr getGlobalState(void) const
			{
				assert(!globalStates.empty() && "Stack underflow");
				return globalStates.top();
			}

			void update(Args... args)
			{
				getGlobalState()->execute(owner, args...);
				getCurrentState()->execute(owner, args...);
			}

			void changeState(StatePtr newState)
			{
				assert(newState && "Invalid state");
				getCurrentState()->exit(owner);
				states.pop();
				states.push(newState);
				getCurrentState()->enter(owner);
			}

			template <typename State, typename... StateArgs>
			void changeState(StateArgs &&...args)
			{
				changeState(std::make_shared<State>(std::forward<StateArgs>(args)...));
			}

			void changeGlobalState(StatePtr newState)
			{
				assert(newState && "Invalid state");
				getGlobalState()->exit(owner);
				globalStates.pop();
				globalStates.push(newState);
				getGlobalState()->enter(owner);
			}

			template <typename State, typename... StateArgs>
			void changeGlobalState(StateArgs &&...args)
			{
				changeGlobalState(std::make_shared<State>(std::forward<StateArgs>(args)...));
			}

			void addState(StatePtr newState)
			{
				assert(newState && "Invalid state");
				getCurrentState()->exit(owner);
				states.push(newState);
				getCurrentState()->enter(owner);
			}

			template <typename State, typename... StateArgs>
			void addState(StateArgs &&...args)
			{
				addState(std::make_shared<State>(std::forward<StateArgs>(args)...));
			}

			void addGlobalState(StatePtr newState)
			{
				assert(newState && "Invalid state");
				getGlobalState()->exit(owner);
				globalStates.push(newState);
				getGlobalState()->enter(owner);
			}

			template <typename State, typename... StateArgs>
			void addGlobalState(StateArgs &&...args)
			{
				addGlobalState(std::make_shared<State>(std::forward<StateArgs>(args)...));
			}

			void revertToPreviousState(std::size_t depth = 1)
			{
				assert(depth < states.size() && "Stack underflow");
				getCurrentState()->exit(owner);
				for (std::size_t i = 0; i < depth; ++i)
				{
					states.pop();
				}
				getCurrentState()->enter(owner);
			}

			void revertToPreviousGlobalState(std::size_t depth = 1)
			{
				assert(depth < globalStates.size() && "Stack underflow");
				getGlobalState()->exit(owner);
				for (std::size_t i = 0; i < depth; ++i)
				{
					globalStates.pop();
				}
				getGlobalState()->enter(owner);
			}

			std::size_t numberOfStates(void) const
			{
				return states.size();
			}

			std::size_t numberOfGlobalStates(void) const
			{
				return globalStates.size();
			}

			bool isInState(ConstStatePtr state) const
			{
				assert(state && "Invalid state");
				return typeid(*getCurrentState().get()) == typeid(*state.get());
			}

			bool isInGlobalState(ConstStatePtr state) const
			{
				assert(state && "Invalid state");
				return typeid(*getGlobalState().get()) == typeid(*state.get());
			}

			template <class State>
			bool isInState(void) const
			{
				return typeid(*getCurrentState().get()) == typeid(State);
			}

			template <class State>
			bool isInGlobalState(void) const
			{
				return typeid(*getGlobalState().get()) == typeid(State);
			}
		};
	}
}
