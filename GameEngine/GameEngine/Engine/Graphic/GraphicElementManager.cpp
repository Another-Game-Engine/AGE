#include "GraphicElementManager.hpp"

#include "BFC/BFCKey.hpp"

#include "AssetManagement/Instance/MeshInstance.hh"

#include "BFC/BFCBlockManager.hpp"

namespace AGE
{
	struct MeshInstance;

	BFCKey GraphicElementManager::addMesh(std::shared_ptr<MeshInstance> meshInstance)
	{
		DRBMesh *drbMesh = _meshPool.create();
		drbMesh->subMeshs = meshInstance->subMeshs;

		BFCKey result;
		
		_bfcBlockManager->createItem((void*)(drbMesh), result.blockId, result.itemId);
		// We don't set the type in the key, it's not the job of GraphicElementManager,
		// it has to be done by the client who ask for the creation

		result.elementPtr = (void*)(drbMesh);
		return result;
	}
}