#pragma once

#include <memory>

#include "Utils/ObjectPool.hpp"
#include "Utils/Dependency.hpp"

#include "DRBPointLight.hpp"
#include "DRBSpotLight.hpp"

namespace AGE
{
	class BFCCullableHandle;
	class BFCBlockManagerFactory;
	class IProperty;

	// That's a SCENE dependency, and not a global one, set on per scene
	class DRBLightElementManager : public Dependency<DRBLightElementManager>
	{
	public:
		DRBLightElementManager(BFCBlockManagerFactory *factory);
		BFCCullableHandle addPointLight();
		BFCCullableHandle addSpotLight();
		void removePointLight(BFCCullableHandle &handle);
		void removeSpotLight(BFCCullableHandle &handle);
	private:
		BFCBlockManagerFactory *_bfcBlockManager = nullptr;
		ObjectPool<DRBPointLight> _pointLightPool;
		ObjectPool<DRBSpotLight> _spotLightPool;
	};
}