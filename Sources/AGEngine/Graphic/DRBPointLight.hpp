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
		virtual glm::vec4 setBFCTransform(const glm::mat4 &transformation) override;
		void setRange(const glm::vec4 &range);
		void updateRange(const glm::mat4 &transformation);

		inline const glm::vec3 &getRange() const { return _range; }
		inline const glm::vec3 &getPosition() const { return _position; }
		inline const glm::mat4 &getSphereTransform() const { return _sphereTransform; }
		inline const glm::vec3 &getColorLight() const { return _colorLight; }
		inline const glm::vec3 &getAmbiantColor() const { return _ambiantColor; }

		inline void setColorLight(const glm::vec4 &color) { _colorLight = glm::vec3(color); }
		inline void setAmbiantColor(const glm::vec4 &color) { _ambiantColor = glm::vec3(color); }
	private:
		glm::vec3 _range;
		glm::mat4 _sphereTransform;
		glm::vec3 _position;
		glm::vec3 _colorLight;
		glm::vec3 _ambiantColor;
	};
}