#include <Components/DirectionalLightComponent.hh>
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

	DirectionalLightData::DirectionalLightData(glm::vec3 const &_color)
		: color(_color)
	{

	}

	DirectionalLightComponent::DirectionalLightComponent()
		: _data(glm::vec3(1.0f))
	{
	}

	DirectionalLightComponent::~DirectionalLightComponent()
	{
	}

	void DirectionalLightComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const DirectionalLightComponent*>(model);
		_data.color = o->_data.color;
		postUnserialization();
	}

	void DirectionalLightComponent::reset()
	{
		if (!_key.invalid())
		{
			entity->getLink().unregisterOctreeObject(_key);
		}
		_key = AGE::PrepareKey();
		_data.color = glm::vec3(1);
	}

	void DirectionalLightComponent::init()
	{
		_key = AGE::GetPrepareThread()->addDirectionalLight();
		entity->getLink().registerOctreeObject(_key);
		assert(!_key.invalid());
		set(_data);
	}

	DirectionalLightComponent &DirectionalLightComponent::set(DirectionalLightData const &data)
	{
		_data = data;
		AGE::GetPrepareThread()->setDirectionalLight(data, _key);
		return (*this);
	}

	void DirectionalLightComponent::postUnserialization()
	{
		init();
		set(_data);
	}

#ifdef EDITOR_ENABLED
	void DirectionalLightComponent::editorCreate()
	{}

	void DirectionalLightComponent::editorDelete()
	{}

	bool DirectionalLightComponent::editorUpdate()
	{
		bool modified = false;
		if (ImGui::ColorEdit3("Color", glm::value_ptr(_data.color)))
		{
			set(_data);
			modified = true;
		}
		return modified;
	}
#endif
}