#include <Render/LightManager.hh>
#include <assert.h>

namespace gl
{
	LightManager::LightManager()
	{

	}

	LightManager::~LightManager()
	{

	}

	DirectionalLight *LightManager::getDirectionalLight(Key<DirectionalLight> const &key)
	{
		if (!key)
			assert(0);
		if (_directionalLight.size() == 0)
			assert(0);
		if (key == _optimizeSearchDirectionalLight.first)
			return ((DirectionalLight *)_light[_optimizeSearchDirectionalLight.second]);
		auto &index = _directionalLight.find(key);
		if (index == _directionalLight.end())
			assert(0);
		_optimizeSearchDirectionalLight.first = key;
		_optimizeSearchDirectionalLight.second = index->second;
		return ((DirectionalLight *)_light[index->second]);
	}

	PointLight *LightManager::getPointLight(Key<PointLight> const &key)
	{
		if (!key)
			assert(0);
		if (_pointLight.size() == 0)
			assert(0);
		if (key == _optimizeSearchPointLight.first)
			return ((PointLight *)_light[_optimizeSearchPointLight.second]);
		auto &index = _pointLight.find(key);
		if (index == _pointLight.end())
			assert(0);
		_optimizeSearchPointLight.first = key;
		_optimizeSearchPointLight.second = index->second;
		return ((PointLight *)_light[index->second]);
	}

	SpotLight *LightManager::getSpotLight(Key<SpotLight> const &key)
	{
		if (!key)
			assert(0);
		if (_spotLight.size() == 0)
			assert(0);
		if (key == _optimizeSearchSpotLight.first)
			return ((SpotLight *)_light[_optimizeSearchSpotLight.second]);
		auto &index = _spotLight.find(key);
		if (index == _spotLight.end())
			assert(0);
		_optimizeSearchSpotLight.first = key;
		_optimizeSearchSpotLight.second = index->second;
		return ((SpotLight *)_light[index->second]);
	}

	size_t LightManager::addPoolLight()
	{
		for (size_t index = 0; index < _light.size(); ++index)
		{
			if (_light[index] == NULL)
				return (index);
		}
		_light.push_back(NULL);
		return (_light.size() - 1);
	}

	Key<DirectionalLight> LightManager::addDirectionalLight(glm::vec4 const &dir, glm::vec4 const &color)
	{
		Key<DirectionalLight> key;
		size_t index = _directionalLight[key] = addPoolLight();
		_light[index] = new DirectionalLight(dir, color);
		return (key);
	}

	Key<DirectionalLight> LightManager::addDirectionalLight(glm::vec4 const &dir)
	{
		return (addDirectionalLight(dir, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	}

	LightManager &LightManager::rmDirectionalLight(Key<DirectionalLight> const &key)
	{
		if (!key)
			assert(0);
		if (_directionalLight.size() == 0)
			assert(0);
		auto &index = _directionalLight.find(key);
		if (index == _directionalLight.end())
			assert(0);
		delete _light[index->second];
		_directionalLight.erase(index);
		return (*this);
	}

	Key<PointLight> LightManager::addPointLight(glm::vec4 const &position, float fallOff, glm::vec4 const &color)
	{
		Key<PointLight> key;

		size_t index = _pointLight[key] = addPoolLight();
		_light[index] = new PointLight(position, fallOff, color);
		return (key);
	}

	Key<PointLight> LightManager::addPointLight(glm::vec4 const &position, float fallOff)
	{
		return (addPointLight(position, fallOff, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	}

	Key<PointLight> LightManager::addPointLight(glm::vec4 const &position)
	{
		return (addPointLight(position, -1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	}

	LightManager &LightManager::rmPointLight(Key<PointLight> const &key)
	{
		if (!key)
			assert(0);
		if (_pointLight.size() == 0)
			assert(0);
		auto &index = _pointLight.find(key);
		if (index == _pointLight.end())
			assert(0);
		delete _light[index->second];
		_pointLight.erase(index);
		return (*this);
	}

	Key<SpotLight> LightManager::addSpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff, float fallOff, glm::vec4 const &color)
	{
		Key<SpotLight> key;

		size_t index = _spotLight[key] = addPoolLight();
		_light[index] = new SpotLight(position, dir, spotCutOff, fallOff, color);
		return (key);
	}
	
	Key<SpotLight> LightManager::addSpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff, float fallOff)
	{
		return (addSpotLight(position, dir, spotCutOff, fallOff));
	}

	Key<SpotLight> LightManager::addSpotLight(glm::vec4 const &position, glm::vec4 const &dir, float spotCutOff)
	{
		return (addSpotLight(position, dir, spotCutOff));
	}

	LightManager &LightManager::rmSpotLight(Key<SpotLight> const &key)
	{
		if (!key)
			assert(0);
		if (_spotLight.size() == 0)
			assert(0);
		auto &index = _spotLight.find(key);
		if (index == _spotLight.end())
			assert(0);
		delete _light[index->second];
		_spotLight.erase(index);
		return (*this);
	}

	LightManager &LightManager::setColorLight(Key<DirectionalLight> const &key, glm::vec4 const &color)
	{
		DirectionalLight *light;

		if ((light = getDirectionalLight(key)) == NULL)
			return (*this);
		light->setColor(color);
		return (*this);
	}

	LightManager &LightManager::setColorLight(Key<SpotLight> const &key, glm::vec4 const &color)
	{
		SpotLight *light;

		if ((light = getSpotLight(key)) == NULL)
			return (*this);
		light->setColor(color);
		return (*this);
	}

	LightManager &LightManager::setColorLight(Key<PointLight> const &key, glm::vec4 const &color)
	{
		PointLight *light;

		if ((light = getPointLight(key)) == NULL)
			return (*this);
		light->setColor(color);
		return (*this);
	}

	LightManager &LightManager::setFallOffPointLight(Key<PointLight> const &key, float fallOff)
	{
		PointLight *light;

		if ((light = getPointLight(key)) == NULL)
			return (*this);
		light->setFallOff(fallOff);
		return (*this);
	}

	LightManager &LightManager::setFallOffSpotLight(Key<SpotLight> const &key, float fallOff)
	{
		SpotLight *light;

		if ((light = getSpotLight(key)) == NULL)
			return (*this);
		light->setFallOff(fallOff);
		return (*this);
	}

	LightManager &LightManager::setSpotCutOffSpotLight(Key<SpotLight> const &key, float spotCutOff)
	{
		SpotLight *light;

		if ((light = getSpotLight(key)) == NULL)
			return (*this);
		light->setSpotCutOff(spotCutOff);
		return (*this);
	}

	LightManager &LightManager::setPositionPointLight(Key<PointLight> const &key, glm::vec4 const &position)
	{
		PointLight *light;

		if ((light = getPointLight(key)) == NULL)
			return (*this);
		light->setPosition(position);
		return (*this);
	}

	LightManager &LightManager::setDirectionDirectionalLight(Key<DirectionalLight> const &key, glm::vec4 const &dir)
	{
		DirectionalLight *light;

		if ((light = getDirectionalLight(key)) == NULL)
			return (*this);
		light->setDirection(dir);
		return (*this);
	}

	LightManager &LightManager::setTransformSpotLight(Key<SpotLight> const &key, glm::vec4 const &position, glm::vec4 const &dir)
	{
		SpotLight *light;

		if ((light = getSpotLight(key)) == NULL)
			return (*this);
		light->setTransform(position, dir);
		return (*this);
	}
}