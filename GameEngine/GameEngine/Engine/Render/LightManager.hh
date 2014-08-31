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

		// add and rm light
		Key<DirectionalLight> addDirectionalLight(glm::vec4 const &dir, glm::vec4 const &color);
		Key<DirectionalLight> addDirectionalLight(glm::vec4 const &dir);
		LightManager &rmDirectionalLight(Key<DirectionalLight> const &key);
		Key<PointLight> addPointLight(glm::vec4 const &position, float fallOff, glm::vec4 const &color);
		Key<PointLight> addPointLight(glm::vec4 const &position, float fallOff);
		Key<PointLight> addPointLight(glm::vec4 const &position);
		LightManager &rmPointLight(Key<PointLight> const &key);
		Key<SpotLight> addSpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff, float fallOff, glm::vec4 const &color);
		Key<SpotLight> addSpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff, float fallOff);
		Key<SpotLight> addSpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff);
		LightManager &rmSpotLight(Key<SpotLight> const &key);

		// setting light
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

		std::pair<Key<DirectionalLight>, size_t> _optimizeSearchDirectionalLight;
		std::pair<Key<SpotLight>, size_t> _optimizeSearchSpotLight;
		std::pair<Key<PointLight>, size_t> _optimizeSearchPointLight;


		DirectionalLight *getDirectionalLight(Key<DirectionalLight> const &key);
		PointLight *getPointLight(Key<PointLight> const &key);
		SpotLight *getSpotLight(Key<SpotLight> const &key);
		size_t addPoolLight();
	};
}