#pragma once

#include <memory>

#include <Utils/ObjectPool.hpp>
#include "DRBMesh.hpp"

namespace AGE
{
	class BFCCullableHandle;
	struct MeshInstance;

	class BFCBlockManager;

	class GraphicElementManager
	{
	public:
		BFCCullableHandle addMesh(std::shared_ptr<MeshInstance> meshInstance);
	private:
		
		BFCBlockManager *_bfcBlockManager = nullptr;

		ObjectPool<DRBMesh> _meshPool;
	};
}