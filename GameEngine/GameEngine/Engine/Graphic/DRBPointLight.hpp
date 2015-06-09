#pragma once

#include "BFC/BFCCullableObject.hpp"

#include "BFCCullableTypes.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

namespace AGE
{
	struct DRBPointLightData;

	struct DRBPointLight : public BFCCullableObject
	{
	public:
		DRBPointLight();
		virtual CullableTypeID getBFCType() const { return BFCCullableType::CullablePointLight; }
		virtual const std::shared_ptr<DRBData> getDatas() const;

		std::shared_ptr<DRBPointLightData> datas;
	};
}