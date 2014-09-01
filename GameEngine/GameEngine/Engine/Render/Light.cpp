#include <Render/Light.hh>

namespace gl
{
	Light::Light(glm::vec4 const &color)
		: _color(color)
	{

	}

	Light::~Light()
	{

	}

	Light &Light::setColor(glm::vec4 const &color)
	{
		_color = color;
		return (*this);
	}

	DirectionalLight::DirectionalLight(glm::vec4 const &dir, glm::vec4 const &color)
		: Light(color),
		_direction(dir)
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}

	DirectionalLight &DirectionalLight::setDirection(glm::vec4 const &dir)
	{
		_direction = dir;
		return (*this);
	}

	PointLight::PointLight(glm::vec4 const &position, float fallOff, glm::vec4 const &color)
		: Light(color),
		_position(position),
		_fallOff(fallOff)
	{

	}

	PointLight::~PointLight()
	{

	}

	PointLight &PointLight::setPosition(glm::vec4 const &position)
	{
		_position = position;
		return (*this);
	}

	PointLight &PointLight::setFallOff(float fallOff)
	{
		_fallOff = fallOff;
		return (*this);
	}

	SpotLight::SpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff, float fallOff, glm::vec4 const &color)
		: Light(color),
		_position(position),
		_direction(dir),
		_spotCutOff(spotCutOff),
		_fallOff(fallOff)
	{

	}

	SpotLight::~SpotLight()
	{

	}

	SpotLight &SpotLight::setTransform(glm::vec4 const &position, glm::vec4 const &dir)
	{
		_position = position;
		_direction = dir;
		return (*this);
	}

	SpotLight &SpotLight::setSpotCutOff(float spotCutOff)
	{
		_spotCutOff = spotCutOff;
		return (*this);
	}

	SpotLight &SpotLight::setFallOff(float fallOff)
	{
		_fallOff = fallOff;
		return (*this);
	}

}