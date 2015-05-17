#pragma once

#include <string>

namespace AGE
{
	namespace Physics
	{
		class MaterialInterface
		{
		public:
			// Constructors
			MaterialInterface(void) = delete;

			MaterialInterface(const std::string &name);

			MaterialInterface(const MaterialInterface &) = delete;

			// Assignment Operators
			MaterialInterface &operator=(const MaterialInterface &) = delete;

			// Methods
			const std::string &getName(void) const;

			// Virtual Methods
			virtual void setStaticFriction(float staticFriction) = 0;

			virtual float getStaticFriction(void) const = 0;

			virtual void setDynamicFriction(float dynamicFriction) = 0;

			virtual float getDynamicFriction(void) const = 0;

			virtual void setRestitution(float restitution) = 0;

			virtual float getRestitution(void) const = 0;

		protected:
			// Static Methods
			static float GetDefaultStaticFriction(void);

			static float GetDefaultDynamicFriction(void);

			static float GetDefaultRestitution(void);

			// Destructor
			virtual ~MaterialInterface(void) = default;

		private:
			// Attributes
			const std::string name;
		};
	}
}

#include "MaterialInterface.inl"