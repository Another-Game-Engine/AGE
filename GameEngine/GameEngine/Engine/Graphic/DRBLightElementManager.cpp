#include "DRBLightElementManager.hpp"
#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

#include "BFC/BFCCullableHandle.hpp"
#include "BFC/BFCBlockManagerFactory.hpp"

namespace AGE
{
	DRBLightElementManager::DRBLightElementManager(BFCBlockManagerFactory *factory)
		: _bfcBlockManager(factory)
	{
		AGE_ASSERT(factory != nullptr);
	}

	BFCCullableHandle DRBLightElementManager::addPointLight(std::shared_ptr<IProperty> color, std::shared_ptr<IProperty> textureMap)
	{
		SCOPE_profile_cpu_function("DRB");

		auto *drbPointLight = _pointLightPool.create();
		drbPointLight->getDatas()->globalProperties.add_property(color);
		drbPointLight->getDatas()->globalProperties.add_property(textureMap);

		BFCCullableHandle result = _bfcBlockManager->createItem(drbPointLight);

		return result;
	}
	
	void DRBLightElementManager::removePointLight(BFCCullableHandle &handle)
	{
		SCOPE_profile_cpu_function("DRB");

		_pointLightPool.destroy(handle.getPtr());
		_bfcBlockManager->deleteItem(handle);
	}
}
