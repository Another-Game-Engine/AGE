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

			std::vector<const char *> _assetRigidBodyList;
			std::vector<const char *> _assetRenderMeshList;
			std::vector<const char *> _assetMateriallist;

			virtual void updateBegin(double time);
			virtual void updateEnd(double time);
			virtual void mainUpdate(double time);
			virtual bool initialize();
		};
	}
}