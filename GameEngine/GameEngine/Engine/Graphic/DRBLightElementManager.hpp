#pragma once

#include <memory>

#include "Utils/ObjectPool.hpp"
#include "DRBPointLight.hpp"
#include "Utils/Dependency.hpp"

namespace AGE
{
	class BFCCullableHandle;
	class BFCBlockManagerFactory;

	// That's a SCENE dependency, and not a global one, set on per scene
	class DRBLightElementManager : public Dependency<DRBLightElementManager>
	{
	public:
		GraphicElementManager(BFCBlockManagerFactory *factory);
		BFCCullableHandle addPointLight();
		void removePointLight(BFCCullableHandle &handle);
	private:
		BFCBlockManagerFactory *_bfcBlockManager = nullptr;
		ObjectPool<DRBPointLight> _pointLightPool;
	};
}