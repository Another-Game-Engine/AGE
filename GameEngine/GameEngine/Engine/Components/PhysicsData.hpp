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
			void init();
			void init(Entity entity);

			Physics::Private::GenericData *getData(void);

			const Physics::Private::GenericData *getData(void) const;

			virtual bool doSerialize() { return false; }

			virtual void _copyFrom(const ComponentBase *model);

			template <typename Archive>
			void save(Archive &, const std::uint32_t) const{}

			template <typename Archive>
			void load(Archive &, const std::uint32_t){}

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