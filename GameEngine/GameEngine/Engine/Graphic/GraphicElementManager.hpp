#pragma once

#include <memory>

#include "Utils/ObjectPool.hpp"
#include "DRBMesh.hpp"
#include "Utils/Dependency.hpp"

namespace AGE
{
	struct MeshInstance;
	struct MaterialSetInstance;

	class BFCCullableHandle;
	class BFCBlockManagerFactory;

	// That's a SCENE dependency, and not a global one, set on per scene
	class GraphicElementManager : public Dependency<GraphicElementManager>
	{
	public:
		GraphicElementManager(BFCBlockManagerFactory *factory);
		BFCCullableHandle addMesh(std::shared_ptr<MeshInstance> meshInstance, std::shared_ptr<MaterialSetInstance> materialInstance);
		void removeMesh(BFCCullableHandle &handle);
	private:
		BFCBlockManagerFactory *_bfcBlockManager = nullptr;
		ObjectPool<DRBMesh> _meshPool;
	};
}