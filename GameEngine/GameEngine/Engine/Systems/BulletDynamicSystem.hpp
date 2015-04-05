#pragma once

#include <Systems/System.h>
#include <Physics/BulletDynamicManager.hpp>

namespace AGE
{
	class BulletDynamicSystem final : public System
	{
	public:
		// Constructors
		BulletDynamicSystem(void) = delete;

		BulletDynamicSystem(AScene *scene);

		BulletDynamicSystem(const BulletDynamicSystem &) = delete;

		// Assignment Operators
		BulletDynamicSystem &operator=(const BulletDynamicSystem &) = delete;

		// Destructors
		virtual ~BulletDynamicSystem(void) = default;

	private:
		// Attributes
		BulletDynamicManager* _manager;

		EntityFilter _filter;

		// Inherited Methods
		virtual void updateBegin(float time) override final;

		// Methods
		void registerContactInformations(void) const;
	};
}