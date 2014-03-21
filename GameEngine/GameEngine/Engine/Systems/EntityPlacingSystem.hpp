#include <Systems/System.h>
#include <Core/Input.hh>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>
#include <Utils/MatrixConversion.hpp>

class EntityPlacingSystem : public System
{
public:
	EntityPlacingSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _z(0.2f)
	{
		_input = _scene.lock()->getInstance<Input>();
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
	float _z;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{

		_z += _input->getMouseWheel().y * 0.1f;
		
		if (_input->getInput(SDL_BUTTON_LEFT))
		{
			glm::vec3 from, to;
			_scene.lock()->getSystem<CameraSystem>()->getRayFromCenterOfScreen(from, to);
			glm::mat4 t = _entity->getLocalTransform();
			glm::vec3 pos = from;
			pos += to * _z;
			t[3][0] = pos.x;
			t[3][1] = pos.y;
			t[3][2] = pos.z;
			t[3][3] = 1;
			_entity->setLocalTransform(t, true);
		}

		if (_input->getInput(SDLK_t))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), -45.0f, glm::vec3(0, 1, 0)), true);
		}

		if (_input->getInput(SDLK_y))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), 45.0f, glm::vec3(0, 1, 0)), true);
		}

		if (_input->getInput(SDLK_b))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), -45.0f, glm::vec3(1, 0, 0)), true);
		}

		if (_input->getInput(SDLK_n))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), 45.0f, glm::vec3(1, 0, 0)), true);
		}

		if (_input->getInput(SDLK_g))
		{
			_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(0.95f)), true);
		}

		if (_input->getInput(SDLK_h))
		{
			_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1.05f)), true);
		}
	}

	virtual void initialize()
	{
	}
};