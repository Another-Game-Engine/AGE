#pragma once

#include <memory>

#include "Utils/ObjectPool.hpp"
#include "DRBPointLight.hpp"
#include "Utils/Dependency.hpp"

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
		BFCCullableHandle addPointLight(std::shared_ptr<IProperty> color, std::shared_ptr<IProperty> textureMap);
		void removePointLight(BFCCullableHandle &handle);
	private:
		BFCBlockManagerFactory *_bfcBlockManager = nullptr;
		ObjectPool<DRBPointLight> _pointLightPool;
	};
}