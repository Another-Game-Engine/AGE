#pragma once

#include <Render/Properties/AProperty.hh>
#include <Render/Properties/Materials/Ratio.hh>
#include <Render/Properties/Materials/Color.hh>
#include <Render/Properties/Materials/MapColor.hh>

namespace AGE
{
	class Texture2D;

	class SpotLightProperty : public AProperty
	{
	public:
		SpotLightProperty();
	private:
		virtual void _update(IProgramResources *p) override final;

	public:
		std::shared_ptr<Texture2D> getTexMap() const;
		void setTexMap(std::shared_ptr<Texture2D> const &m);

		glm::mat4 getMatrix() const;
		void setMatrix(const glm::mat4 &matrix);
		
		glm::vec3 getPosition() const;
		void setPosition(const glm::vec3 &position);

		glm::vec3 getAttenuation() const;
		void setAttenuation(const glm::vec3 &attenuation);

		glm::vec3 getDirection() const;
		void setDirection(const glm::vec3 &direction);

		float getSpotCutOff() const;
		void setSpotCutOff(float cutOff);

		glm::vec3 getColor() const;
		void setColor(const glm::vec3 &color);

	private:
		std::shared_ptr<Texture2D> _propShadowMap;
		glm::mat4                  _propShadowMatrix;
		glm::vec3                  _propPosition;
		glm::vec3                  _propAttenuation;
		glm::vec3                  _propDirection;
		float                      _propSpotCutOff;
		float                      _propExponentLight;
		glm::vec3                  _propColorLight;


		Ratio _ratio;
		Color _color;
		MapColor _mapColor;
	};
}
