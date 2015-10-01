#include "GraphicElementManager.hpp"

#include "BFC/BFCCullableHandle.hpp"
#include "BFC/BFCBlockManagerFactory.hpp"

#include "Graphic/DRBMeshData.hpp"

#include "AssetManagement/Instance/MeshInstance.hh"
#include "AssetManagement/Instance/MaterialInstance.hh"
#include <Render/Properties/SkeletonProperty.hpp>

namespace AGE
{
	GraphicElementManager::GraphicElementManager(BFCBlockManagerFactory *factory)
		: _bfcBlockManager(factory)
	{
		AGE_ASSERT(factory != nullptr);
	}

	BFCCullableHandleGroup GraphicElementManager::addMesh(std::shared_ptr<MeshInstance> mesh, std::shared_ptr<MaterialSetInstance> materialInstance)
	{
		SCOPE_profile_cpu_function("DRB");

		BFCCullableHandleGroup result;

		std::shared_ptr<SkeletonProperty> skeletonProperty = nullptr;

		for (auto &submesh : mesh->subMeshs)
		{
			DRBMesh *drbMesh = _meshPool.create();

			drbMesh->datas->setVerticesKey(submesh.vertices);
			drbMesh->datas->setPainterKey(submesh.painter);
			drbMesh->datas->setAABB(submesh.boundingBox);

			drbMesh->datas->globalProperties.merge_properties(submesh.properties);

			if (submesh.isSkinned)
			{
				if (skeletonProperty == nullptr)
				{
					skeletonProperty = std::make_shared<SkeletonProperty>();
				}
				drbMesh->datas->globalProperties.add_property(skeletonProperty);
			}

			std::size_t materialIndex = submesh.defaultMaterialIndex < materialInstance->datas.size() ? submesh.defaultMaterialIndex : 0;
			auto &material = materialInstance->datas[materialIndex];

			for (auto &p : material._properties)
			{
				drbMesh->datas->globalProperties.add_property(p);
			}

			BFCCullableHandle resultMesh = _bfcBlockManager->createItem(drbMesh);
			result.getHandles().push_back(resultMesh);
		}
		return result;
	}

	void GraphicElementManager::removeMesh(BFCCullableHandleGroup &handle)
	{
		SCOPE_profile_cpu_function("DRB");

		for (auto &m : handle.getHandles())
		{
			_meshPool.destroy(m.getPtr());
			_bfcBlockManager->deleteItem(m);
		}
		handle.getHandles().clear();
	}
}