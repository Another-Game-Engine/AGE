#pragma once

#include <memory>

#include "Utils/ObjectPool.hpp"
#include "DRBMesh.hpp"
#include "Utils/Dependency.hpp"

namespace AGE
{
	struct MeshInstance;
	struct SubMeshInstance;
	struct MaterialSetInstance;

	class BFCCullableHandle;
	class BFCCullableHandleGroup;
	class BFCBlockManagerFactory;

	// That's a SCENE dependency, and not a global one, set on per scene
	class GraphicElementManager : public Dependency<GraphicElementManager>
	{
	public:
		GraphicElementManager(BFCBlockManagerFactory *factory);
		BFCCullableHandleGroup addMesh(std::shared_ptr<MeshInstance> meshInstance, std::shared_ptr<MaterialSetInstance> materialInstance);
		void removeMesh(BFCCullableHandleGroup &handle);
	private:
		BFCBlockManagerFactory *_bfcBlockManager = nullptr;
		ObjectPool<DRBMesh> _meshPool;
	};
}