#pragma once

#include <Render/Light.hh>
#include <Utils/Containers/Vector.hpp>
#include <Render/UniformBlock.hh>
#include <map>

namespace gl
{
	struct LayoutLight;

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
		LightManager &setUniformBlock(Key<PointLight> const &key, UniformBlock &ubo, size_t index);
		LightManager &setUniformBlock(Key<SpotLight> const &key, UniformBlock &ubo, size_t index);
		LightManager &setUniformBlock(Key<DirectionalLight> const &key, UniformBlock &ubo, size_t index);

		// add Layout Light
		//Key<LayoutLight> addLayoutLight();
		//LightManager &rmLayoutLight(Key<LayoutLight> const &key);
		//LightManager &pushLightLayoutLight(Key<LayoutLight> const &layoutLight, Key<DirectionalLight> const &light);
		//LightManager &pushLightLayoutLight(Key<LayoutLight> const &layoutLight, Key<PointLight> const &light);
		//LightManager &pushLightLayoutLight(Key<LayoutLight> const &layoutLight, Key<SpotLight> const &light);
		//LightManager &popLightLayoutLight(Key<LayoutLight> const &layoutLight, Key<DirectionalLight> const &light);
		//LightManager &popLightLayoutLight(Key<LayoutLight> const &layoutLight, Key<PointLight> const &light);
		//LightManager &popLightLayoutLight(Key<LayoutLight> const &layoutLight, Key<SpotLight> const &light);
	//	LightManager &setUniformBlock(Key<LayoutLight> const &key, UniformBlock &ubo);

	private:
		LightManager(LightManager const &copy) = delete;
		LightManager &operator=(LightManager const &l) = delete;

		AGE::Vector<Light *> _light;
		std::map<Key<DirectionalLight>, size_t> _directionalLight;
		std::map<Key<PointLight>, size_t> _pointLight;
		std::map<Key<SpotLight>, size_t> _spotLight;
		//std::map<Key<LayoutLight>, LayoutLight> _layoutLight;

		std::pair<Key<DirectionalLight>, size_t> _optimizeSearchDirectionalLight;
		std::pair<Key<SpotLight>, size_t> _optimizeSearchSpotLight;
		std::pair<Key<PointLight>, size_t> _optimizeSearchPointLight;
		//std::pair<Key<LayoutLight>, LayoutLight *> _optimizeSearchLayoutLight;

		size_t getLight(Key<DirectionalLight> const &key);
		size_t getLight(Key<PointLight> const &key);
		size_t getLight(Key<SpotLight> const &key);
		DirectionalLight *getDirectionalLight(Key<DirectionalLight> const &key);
		PointLight *getPointLight(Key<PointLight> const &key);
		SpotLight *getSpotLight(Key<SpotLight> const &key);
		//LayoutLight *getLayoutLight(Key<LayoutLight> const &key);
		size_t addPoolLight();
	};

	//struct LayoutLight
	//{
	//	AGE::Vector<size_t> _spot;
	//	AGE::Vector<size_t> _point;
	//	AGE::Vector<size_t> _directional;
	//};
}