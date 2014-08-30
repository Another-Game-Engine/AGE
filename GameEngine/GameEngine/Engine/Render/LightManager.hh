#pragma once

#include <Render/Light.hh>
#include <Utils/Containers/Vector.hpp>
#include <map>

namespace gl
{
	class LightManager
	{
	public:
		LightManager();
		~LightManager();

		Key<DirectionalLight> addDirectionalLight();
		Key<PointLight> addPointLight();
		Key<SpotLight> addSpotLight();

		LightManager &setColorLight(Key<DirectionalLight> const &key, glm::vec4 const &color);
		LightManager &setColorLight(Key<SpotLight> const &key, glm::vec4 const &color);
		LightManager &setColorLight(Key<PointLight> const &key, glm::vec4 const &color);
		LightManager &setFallOffPointLight(Key<PointLight> const &key, float fallOff);
		LightManager &setFallOffSpotLight(Key<SpotLight> const &key, float fallOff);
		LightManager &setSpotCutOffSpotLight(Key<SpotLight> const &key, float spotCutOff);
		LightManager &setPositionPointLight(Key<PointLight> const &key, glm::vec4 const &position);
		LightManager &setDirectionDirectionalLight(Key<DirectionalLight> const &key, glm::vec4 const &dir);
		LightManager &setTransformSpotLight(Key<SpotLight> const &key, glm::vec4 const &position, glm::vec4 const &dir);

	private:
		LightManager(LightManager const &copy) = delete;
		LightManager &operator=(LightManager const &l) = delete;
		
		AGE::Vector<Light *> _light;
		std::map<Key<DirectionalLight>, size_t> _directionalLight;
		std::map<Key<PointLight>, size_t> _pointLight;
		std::map<Key<SpotLight>, size_t> _spotLight;
		
	};
}