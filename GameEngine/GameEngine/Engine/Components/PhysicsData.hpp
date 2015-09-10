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

			inline Physics::Private::GenericData *getData(void) { return &data; }

			inline const Physics::Private::GenericData *getData(void) const { return &data; }

			virtual bool doSerialize();

			virtual void _copyFrom(const ComponentBase *model);

			template <typename Archive>
			void save(Archive &, const std::uint32_t) const{}

			template <typename Archive>
			void load(Archive &, const std::uint32_t){}

#ifdef EDITOR_ENABLED
			virtual bool isExposedInEditor();
			bool deletableInEditor = false;
			virtual bool serializeInExport();
#endif

		private:
			// Attributes
			Physics::Private::GenericData data;
		};
	}
}