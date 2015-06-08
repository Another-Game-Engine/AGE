#include <Components/CameraComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/MainThread.hpp>
#include <Context/SDL/SdlContext.hh>
#include <Core/AScene.hh>
#include <imgui/imgui.h>

namespace AGE
{
	CameraComponent::CameraComponent()
		: ComponentBase()
	{
	}

	CameraComponent::~CameraComponent(void)
	{
	}

	void CameraComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const CameraComponent*>(model);
		_data = o->_data;
	}

	void CameraComponent::setProjection(const glm::mat4 &projection)
	{
		_data.projection = projection;
		//AGE::GetPrepareThread()->setCameraInfos(_data, _key);
	}

	void CameraComponent::setPipeline(RenderType pipeline)
	{
		_data.pipeline = pipeline;
		//AGE::GetPrepareThread()->setCameraInfos(_data, _key);
	}

	void CameraComponent::setTexture(std::shared_ptr<Texture3D> const &texture)
	{
		_data.texture = texture;
		//AGE::GetPrepareThread()->setCameraInfos(_data, _key);
	}

	void CameraComponent::init()
	{
		auto scene = entity->getScene();
		//_key = AGE::GetPrepareThread()->addCamera();
		//entity->getLink().registerOctreeObject(_key);
		auto screenSize = entity->getScene()->getInstance<IRenderContext>()->getScreenSize();
		setProjection(glm::perspective(60.0f, (float)screenSize.x / (float)screenSize.y, 0.1f, 2000.0f));
	}

	void CameraComponent::reset()
	{
		if (!_key.invalid())
		{
			//entity->getLink().unregisterOctreeObject(_key);
		}
		_data = CameraData();
	}

	void CameraComponent::postUnserialization()
	{
		init();
		//AGE::GetPrepareThread()->setCameraInfos(_data, _key);
	}

#ifdef EDITOR_ENABLED
	void CameraComponent::addSkyBoxToChoice(std::string const &type, std::shared_ptr<Texture3D> const &texture)
	{
		_choicesSkymap[type] = texture;
	}

	void CameraComponent::editorCreate()
	{}

	void CameraComponent::editorDelete()
	{}

	bool CameraComponent::editorUpdate()
	{
		bool modified = false;
		int countChoices = _choicesSkymap.size();
		char  **choices = new char *[countChoices];
		int index = 0;
		int current_item = 0;
		for (auto &choice : _choicesSkymap) {
			choices[index] = new char[50];
			memset(choices[index], 0, 50);
			char const *tmp = choice.first.c_str();
			memcpy(choices[index], tmp, choice.first.size());
			++index;
		}
		if (ImGui::ListBox("Skybox", &current_item, (char const **)choices, countChoices))
		{
			setTexture(_choicesSkymap[std::string(choices[current_item])]);
			modified = true;
		}
		for (int i = 0; i < countChoices; ++i)
			delete[] choices[i];
		delete[] choices;
		return modified;
	}
#endif

};

