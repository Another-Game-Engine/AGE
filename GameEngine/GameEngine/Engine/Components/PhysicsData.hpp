#pragma once

#include <Components/Component.hh>
#include <Physics/GenericData.hpp>

namespace AGE
{
	namespace Private
	{
		class PhysicsData final : public ComponentBase
		{
			AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_Private_physics_data");
		public:
			// Constructors
			PhysicsData(void) = default;

			PhysicsData(const PhysicsData &) = delete;

			// Assignment Operators
			PhysicsData &operator=(const PhysicsData &) = delete;

			// Destructor
			~PhysicsData(void) = default;

			// Inherited Methods
			void reset(void) override final;

			// Methods
			void init(void);

			Physics::Private::GenericData *getData(void);

			const Physics::Private::GenericData *getData(void) const;

			virtual bool doSerialize() { return false; }

#ifdef EDITOR_ENABLED
			virtual bool isExposedInEditor(){ return false; }
			bool deletableInEditor = false;
			virtual bool serializeInExport() { return false; }
#endif

		private:
			// Attributes
			Physics::Private::GenericData data;
		};
	}
}