#include <Components/SpotLight.hh>
#include <Core/AScene.hh>
#include <Utils/MathematicTools.hh>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <glm/glm.hpp>
#include <AssetManagement/AssetManager.hh>

#ifdef EDITOR_ENABLED
#	include <imgui\imgui.h>
#	include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	SpotLightData::SpotLightData(glm::vec3 const &color, glm::vec3 const &range, float exponent, float cutOff, std::shared_ptr<ITexture> const &map)
		: color(color),
		range(range),
		exponent(exponent),
		cutOff(cutOff),
		map(map)
	{

	}

	SpotLightComponent::SpotLightComponent(SpotLightComponent const &o)
		: _key(o._key)
		, _data(o._data)
	{
		postUnserialization();
	}

	void SpotLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const SpotLightComponent*>(model);
		_data = o->_data;
		postUnserialization();
	}

	void SpotLightComponent::reset()
	{
		if (!_key.invalid())
		{
			entity.getLink().unregisterOctreeObject(_key);
		}
		_key = AGE::PrepareKey();
		_data = SpotLightData();
	}

	void SpotLightComponent::init()
	{
		_key = AGE::GetPrepareThread()->addSpotLight();
		entity.getLink().registerOctreeObject(_key);
		_data.map = entity.getScene()->getInstance<AssetsManager>()->getSpotLightTexture();
		assert(!_key.invalid());
		set(_data);
	}

	void SpotLightComponent::set(SpotLightData const &data)
	{
		_data = data;
		AGE::GetPrepareThread()->setSpotLight(_data, _key);
	}

	void SpotLightComponent::postUnserialization()
	{
		init();
	}

#ifdef EDITOR_ENABLED
	void SpotLightComponent::editorCreate()
	{}

	void SpotLightComponent::editorDelete()
	{
	}

	bool SpotLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(_data.color)))
		{
			set(_data);
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(_data.range), 0.0f, 1.0f))
		{
			set(_data);
			modified = true;
		}
		if (ImGui::InputFloat("Exponent", &_data.exponent))
		{
			set(_data);
			modified = true;
		}
		if (ImGui::InputFloat("cut off", &_data.cutOff))
		{
			set(_data);
			modified = true;
		}
		return modified;
	}
#endif
}