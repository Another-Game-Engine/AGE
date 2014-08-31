#pragma once

#include <glm/glm.hpp>
#include <Render/Key.hh>

namespace gl
{
	class Shader;

	class Light
	{
	public:

		~Light();

		virtual Light &setShader(Shader &shader) = 0;
		Light &setColor(glm::vec4 const &color);

	protected:
		Light(glm::vec4 const &color);
		Light(Light const &copy) = delete;
		Light &operator=(Light const &l) = delete;
		
		glm::vec4 _color;
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(glm::vec4 const &dir, glm::vec4 const &color);
		~DirectionalLight();

		virtual Light &setShader(Shader &shader);
		DirectionalLight &setDirection(glm::vec4 const &dir);

	private:
		DirectionalLight(DirectionalLight const &copy) = delete;
		DirectionalLight &operator=(DirectionalLight const &d) = delete;
	
		glm::vec4 _direction;
	};

	class PointLight : public Light
	{
	public:
		PointLight(glm::vec4 const &position, float fallOff, glm::vec4 const &color);
		~PointLight();

		virtual Light &setShader(Shader &shader);
		PointLight &setPosition(glm::vec4 const &position);
		PointLight &setFallOff(float fallOff);

	private:
		PointLight(PointLight const &copy) = delete;
		PointLight &operator=(PointLight const &p) = delete;

		glm::vec4 _position;
		float _fallOff;
	};

	class SpotLight : public Light
	{
	public:
		SpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff, float fallOff, glm::vec4 const &color);
		~SpotLight();

		virtual Light &setShader(Shader &shader);
		SpotLight &setTransform(glm::vec4 const &position, glm::vec4 const &dir);
		SpotLight &setSpotCutOff(float spotCutOff);
		SpotLight &setFallOff(float fallOff);

	private:
		SpotLight &operator=(SpotLight const &copy) = delete;
		SpotLight(SpotLight const &s) = delete;

		glm::vec4 _position;
		glm::vec4 _direction;
		float _spotCutOff;
		float _fallOff;
	};
}