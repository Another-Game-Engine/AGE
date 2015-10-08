#pragma once

#include "BFC/BFCCullableObject.hpp"

#include "BFCCullableTypes.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

namespace AGE
{
	struct DRBSpotLightData;

	struct DRBSpotLight : public BFCCullableObject
	{
	public:
		DRBSpotLight();
		const std::shared_ptr<DRBSpotLightData> getDatas() const;
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation);
	private:
		std::shared_ptr<DRBSpotLightData> _datas;
	};
}