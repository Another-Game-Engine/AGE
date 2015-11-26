#include "DRBLightElementManager.hpp"
#include "DRBPointLight.hpp"
#include "DRBPointLightData.hpp"

#include "BFC/BFCCullableHandle.hpp"
#include "BFC/BFCBlockManagerFactory.hpp"
#include "Utils/Profiler.hpp"

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

		BFCCullableHandle result = _bfcBlockManager->createItem(drbPointLight);

		return result;
	}
	
	BFCCullableHandle DRBLightElementManager::addSpotLight()
	{
		SCOPE_profile_cpu_function("DRB");

		auto *drbSpotLight = _spotLightPool.create();

		BFCCullableHandle result = _bfcBlockManager->createItem(drbSpotLight);

		return result;
	}

	void DRBLightElementManager::removePointLight(BFCCullableHandle &handle)
	{
		SCOPE_profile_cpu_function("DRB");

		_pointLightPool.destroy(handle.getPtr());
		_bfcBlockManager->deleteItem(handle);
	}

	void DRBLightElementManager::removeSpotLight(BFCCullableHandle &handle)
	{
		SCOPE_profile_cpu_function("DRB");

		_spotLightPool.destroy(handle.getPtr());
		_bfcBlockManager->deleteItem(handle);
	}
}
