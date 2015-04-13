#pragma once

namespace AGE
{
	namespace AI
	{
		template <class Entity, typename... Args>
		class StateMachine;

		template <class Entity, typename... Args>
		class State
		{
			friend class StateMachine < Entity, Args... > ;

		public:
			using EntityType = Entity;

			using EntityPtr = EntityType *;

			using ConstEntityPtr = EntityPtr const;

			// Constructors
			State(void) = default;

			State(State const &) = delete;

			// Assignment operators
			State &operator=(State const &) = delete;

			// Destructor
			virtual ~State(void) = default;

		protected:
			// Virtual Methods
			virtual void enter(EntityPtr entity) = 0;

			virtual void execute(EntityPtr entity, Args... args) = 0;

			virtual void exit(EntityPtr entity) = 0;
		};
	}
}
