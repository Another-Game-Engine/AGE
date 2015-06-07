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

		for (auto &submesh : drbMesh->subMeshs)
		{
			std::size_t materialIndex = submesh.defaultMaterialIndex < materialInstance->datas.size() ? submesh.defaultMaterialIndex : 0;
			auto &material = materialInstance->datas[materialIndex];

			for (auto &matProperty : material._properties)
			{
				submesh.properties.add_property(matProperty);
			}
		}

		BFCCullableHandle result = _bfcBlockManager->createItem(drbMesh);

		return result;
	}

	void GraphicElementManager::removeMesh(BFCCullableHandle &handle)
	{
		_meshPool.destroy(handle.getPtr());
		_bfcBlockManager->deleteItem(handle);
	}

}