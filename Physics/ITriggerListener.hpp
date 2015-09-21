#pragma once

#include <Core/AScene.hh>

namespace AGE
{
	namespace Private
	{
		class TriggerSystem;
	}

	namespace Physics
	{
		class ITriggerListener
		{
			// Friendships
			friend ::AGE::Private::TriggerSystem;

		public:
			// Constructors
			ITriggerListener(void) = delete;

			ITriggerListener(AScene *scene);

			ITriggerListener(const ITriggerListener &) = delete;

			// Assignment Operators
			ITriggerListener &operator=(const ITriggerListener &) = delete;

			// Destructor
			virtual ~ITriggerListener(void);

		protected:
			// Virtual Methods
			virtual void onTriggerEnter(Entity triggerEntity, Entity otherEntity);

			virtual void onTriggerStay(Entity triggerEntity, Entity otherEntity);

			virtual void onTriggerExit(Entity triggerEntity, Entity otherEntity);

		private:
			// Attributes
			AScene *scene = nullptr;
		};
	}
}