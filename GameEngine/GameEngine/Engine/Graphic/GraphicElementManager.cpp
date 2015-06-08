#include "GraphicElementManager.hpp"

#include "BFC/BFCCullableHandle.hpp"
#include "BFC/BFCBlockManagerFactory.hpp"

#include "Graphic/DRBMeshData.hpp"

#include "AssetManagement/Instance/MeshInstance.hh"
#include "AssetManagement/Instance/MaterialInstance.hh"




namespace AGE
{
	GraphicElementManager::GraphicElementManager(BFCBlockManagerFactory *factory)
		: _bfcBlockManager(factory)
	{
		AGE_ASSERT(factory != nullptr);
	}

	BFCCullableHandle GraphicElementManager::addMesh(const SubMeshInstance &meshInstance, std::shared_ptr<MaterialSetInstance> materialInstance)
	{
		SCOPE_profile_cpu_function("DRB");

		DRBMesh *drbMesh = _meshPool.create();

		drbMesh->datas->setVerticesKey(meshInstance.vertices);
		drbMesh->datas->setPainterKey(meshInstance.painter);
		
		// TODO merge Mesh properties with global properties
		//drbMesh->datas->globalProperties = meshInstance.properties;
		
		std::size_t materialIndex = meshInstance.defaultMaterialIndex < materialInstance->datas.size() ? meshInstance.defaultMaterialIndex : 0;
		auto &material = materialInstance->datas[materialIndex];

		for (auto &p : material._properties)
		{
			drbMesh->datas->globalProperties.add_property(p);
		}

		BFCCullableHandle result = _bfcBlockManager->createItem(drbMesh);
		return result;
	}

	void GraphicElementManager::removeMesh(BFCCullableHandle &handle)
	{
		SCOPE_profile_cpu_function("DRB");

		_meshPool.destroy(handle.getPtr());
		_bfcBlockManager->deleteItem(handle);
	}

}