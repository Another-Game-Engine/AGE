#include <Systems/System.h>
#include <Core/Input.hh>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>

class EntityPlacingSystem : public System
{
public:
	EntityPlacingSystem(std::weak_ptr<AScene> scene)
		: System(scene)
	{
		_input = _scene.lock()->getInstance<Input>();
		_fontManager = _scene.lock()->getInstance<FontManager>();
	}

	virtual ~EntityPlacingSystem()
	{}

	void setEntity(Entity &e)
	{
		_entity = e;
	}

private:
	Entity _entity;
	std::shared_ptr<Input> _input;
	std::shared_ptr<FontManager> _fontManager;
	std::shared_ptr<Font> _font;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		if (_input->getInput(SDL_BUTTON_LEFT))
		{
			glm::vec3 from, to;
			_scene.lock()->getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
			glm::mat4 t = _entity->getLocalTransform();
			glm::vec3 pos = from + to * 1.5f;
			t[3][0] = pos.x;
			t[3][1] = pos.y;
			t[3][2] = pos.z;
			t[3][3] = 1;
			_entity->setLocalTransform(t);
		}

		if (_input->getInput(SDLK_t))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), -45.0f, glm::vec3(0, 1, 0)));
		}

		if (_input->getInput(SDLK_y))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), 45.0f, glm::vec3(0, 1, 0)));
		}

		if (_input->getInput(SDLK_g))
		{
			_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(0.95f)));
		}

		if (_input->getInput(SDLK_h))
		{
			_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1.05f)));
		}

		if (!_entity.get())
			return;
		std::string message = "";
		auto m = _entity->getLocalTransform();
		message += std::to_string(m[0][0]) + " ";
		message += std::to_string(m[0][1]) + " ";
		message += std::to_string(m[0][2]) + " ";
		message += std::to_string(m[0][3]) + " ";
		message += std::to_string(m[1][0]) + " ";
		message += std::to_string(m[1][1]) + " ";
		message += std::to_string(m[1][2]) + " ";
		message += std::to_string(m[1][3]) + " ";
		message += std::to_string(m[2][0]) + " ";
		message += std::to_string(m[2][1]) + " ";
		message += std::to_string(m[2][2]) + " ";
		message += std::to_string(m[2][3]) + " ";
		message += std::to_string(m[3][0]) + " ";
		message += std::to_string(m[3][1]) + " ";
		message += std::to_string(m[3][2]) + " ";
		message += std::to_string(m[3][3]) + " ";


		_fontManager->draw2DString(message, "myFont", 20, glm::ivec2(10, 200), glm::vec4(1), "2DText");
	}

	virtual void initialize()
	{
		if (!_fontManager->loadFont(File("../../Assets/Serialized/myFont.cpdFont")))
		{
			// exception
		}
	}
};