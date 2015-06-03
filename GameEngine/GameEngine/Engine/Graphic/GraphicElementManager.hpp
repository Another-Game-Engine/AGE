#pragma once

#include <memory>

#include <Utils/ObjectPool.hpp>
#include "DRBMesh.hpp"

namespace AGE
{
	struct BFCKey;
	struct MeshInstance;

	class BFCBlockManager;

	class GraphicElementManager
	{
	public:
		BFCKey addMesh(std::shared_ptr<MeshInstance> meshInstance);
	private:
		
		BFCBlockManager *_bfcBlockManager = nullptr;

		ObjectPool<DRBMesh> _meshPool;
	};
}