#include "GraphicElementManager.hpp"

#include "BFC/BFCCullableHandle.hpp"

#include "AssetManagement/Instance/MeshInstance.hh"
#include "AssetManagement/Instance/MaterialInstance.hh"

#include "BFC/BFCBlockManagerFactory.hpp"

namespace AGE
{
	GraphicElementManager::GraphicElementManager(BFCBlockManagerFactory *factory)
		: _bfcBlockManager(factory)
	{
		AGE_ASSERT(factory != nullptr);
	}

	BFCCullableHandle GraphicElementManager::addMesh(std::shared_ptr<MeshInstance> meshInstance, std::shared_ptr<MaterialSetInstance> materialInstance)
	{
		DRBMesh *drbMesh = _meshPool.create();
		drbMesh->subMeshs = meshInstance->subMeshs;
		drbMesh->subMaterials = materialInstance->datas;
		
		BFCCullableHandle result = _bfcBlockManager->createItem(drbMesh);

		return result;
	}

	void GraphicElementManager::removeMesh(BFCCullableHandle &handle)
	{
		_meshPool.destroy(handle.getPtr());
		_bfcBlockManager->deleteItem(handle);
	}

}