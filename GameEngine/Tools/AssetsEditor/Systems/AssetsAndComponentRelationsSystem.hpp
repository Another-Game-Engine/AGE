#pragma once

#include <Systems/System.h>

namespace AGE
{
	namespace WE
	{
		class AssetsAndComponentRelationsSystem : public System
		{
		public:
			AssetsAndComponentRelationsSystem(std::weak_ptr<AScene> &&scene);
			virtual ~AssetsAndComponentRelationsSystem();
		private:
			EntityFilter _rigidBodies;
			EntityFilter _meshRenderer;

			virtual void updateBegin(double time);
			virtual void updateEnd(double time);
			virtual void mainUpdate(double time);
			virtual bool initialize();
		};
	}
}