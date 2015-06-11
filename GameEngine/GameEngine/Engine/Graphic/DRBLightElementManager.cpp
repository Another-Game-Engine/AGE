#include "DRBLightElementManager.hpp"

#include "BFC/BFCCullableHandle.hpp"
#include "BFC/BFCBlockManagerFactory.hpp"

namespace AGE
{
	DRBLightElementManager::DRBLightElementManager(BFCBlockManagerFactory *factory)
		: _bfcBlockManager(factory)
	{
		AGE_ASSERT(factory != nullptr);
	}

	BFCCullableHandle DRBLightElementManager::addPointLight()
	{
		SCOPE_profile_cpu_function("DRB");

		auto *drbPointLight = _pointLightPool.create();

		drbPointLight->datas->globalProperties.merge_properties(meshInstance.properties);

		std::size_t materialIndex = meshInstance.defaultMaterialIndex < materialInstance->datas.size() ? meshInstance.defaultMaterialIndex : 0;
		auto &material = materialInstance->datas[materialIndex];

		for (auto &p : material._properties)
		{
			drbPointLight->datas->globalProperties.add_property(p);
		}

		BFCCullableHandle result = _bfcBlockManager->createItem(drbPointLight);
		return result;
	}
	
	void DRBLightElementManager::removePointLight(BFCCullableHandle &handle)
	{

	}
}
