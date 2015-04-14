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
	SpotLightComponent::SpotLightComponent()
		: _range(1)
		, _color(1),
		_map(nullptr)
	{
	}

	SpotLightComponent::~SpotLightComponent()
	{

	}

	SpotLightComponent::SpotLightComponent(SpotLightComponent const &o)
		: _key(o._key)
		, _range(o._range)
		, _color(o._color),
		_map(o._map)
	{
		postUnserialization();
	}

	void SpotLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const SpotLightComponent*>(model);
		_range = o->_range;
		_color = o->_color;
		postUnserialization();
	}

	void SpotLightComponent::reset()
	{
		if (!_key.invalid())
		{
			entity.getLink().unregisterOctreeObject(_key);
		}
		_key = AGE::PrepareKey();
		_color = glm::vec3(1);
		_range = glm::vec3(1);
	}

	void SpotLightComponent::init()
	{
		_key = AGE::GetPrepareThread()->addSpotLight();
		entity.getLink().registerOctreeObject(_key);
		_map = entity.getScene()->getInstance<AssetsManager>()->getPointLightTexture();
		assert(!_key.invalid());
	}

	SpotLightComponent &SpotLightComponent::set(glm::vec3 const &color, glm::vec3 const &range)
	{
		_color = color;
		_range = range;
		AGE::GetPrepareThread()->setPointLight(color, range, _map, _key);
		return (*this);
	}

	void SpotLightComponent::postUnserialization()
	{
		init();
		set(_color, _range);
	}

#ifdef EDITOR_ENABLED
	void SpotLightComponent::editorCreate()
	{}

	void SpotLightComponent::editorDelete()
	{}

	bool SpotLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", getColorPtr()))
		{
			set(_color, _range);
			modified = true;
		}
		if (ImGui::SliderFloat3("Range", glm::value_ptr(_range), 0.0f, 1.0f))
		{
			set(_color, _range);
			modified = true;
		}
		return modified;
	}
#endif
}