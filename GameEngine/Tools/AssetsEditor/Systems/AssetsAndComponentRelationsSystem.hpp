#pragma once

#include <Systems/System.h>

namespace AGE
{
	namespace WE
	{
		class AssetsAndComponentRelationsSystem : public System
		{
		public:
			AssetsAndComponentRelationsSystem(AScene *scene);
			virtual ~AssetsAndComponentRelationsSystem();
		private:
			EntityFilter _rigidBodies;
			EntityFilter _meshRenderer;

			virtual void updateBegin(float time);
			virtual void updateEnd(float time);
			virtual void mainUpdate(float time);
			virtual bool initialize();
		};
	}
}