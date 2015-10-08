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
		const std::shared_ptr<DRBPointLightData> getDatas() const;
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation) override;
	private:
		std::shared_ptr<DRBPointLightData> _datas;
	};
}