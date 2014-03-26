#include <Systems/System.h>
#include <Core/Input.hh>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Components/EntityPlacable.hpp>

class EntityPlacingSystem : public System
{
public:
	EntityPlacingSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _filter(scene)
		, _name("NO SELECTION")
		, _z(0.2f)
	{
		_input = _scene.lock()->getInstance<Input>();
	}

	virtual ~EntityPlacingSystem()
	{}

private:
	EntityFilter _filter;
	std::shared_ptr<Input> _input;
	std::string _name;
	float _z;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	Entity getEntity()
	{
		if (_filter.getCollection().size() == 0)
			return Entity();
		if (_filter.getCollection().size() == 1)
		{
			_name = _filter.getCollection().begin()->get()->getComponent<Component::EntityPlacable>()->name;
			return *_filter.getCollection().begin();
		}
		for (auto e : _filter.getCollection())
		{
			if (e->getComponent<Component::EntityPlacable>()->name == _name)
				return e;
		}
		_name = _filter.getCollection().begin()->get()->getComponent<Component::EntityPlacable>()->name;
		return *_filter.getCollection().begin();
	}

	virtual void mainUpdate(double time)
	{
		static float t = 0.3f;
		_z += _input->getMouseWheel().y * 0.1f;
		
		if (_input->getInput(SDLK_MINUS) && !_filter.getCollection().empty() && t <= 0.0f)
		{
			auto &collection = _filter.getCollection();

			if (collection.size() == 1)
			{
				_name = std::begin(collection)->get()->getComponent<Component::EntityPlacable>()->name;
			}
			else
			{
				for (auto it = std::begin(collection); it != std::end(collection); ++it)
				{
					auto c = it->get()->getComponent<Component::EntityPlacable>();
					if (c->name != _name)
						continue;
					if (it != std::begin(collection))
					{
						_name = (--it)->get()->getComponent<Component::EntityPlacable>()->name;
						break;
					}
					else
					{
						_name = (--(std::end(collection)))->get()->getComponent<Component::EntityPlacable>()->name;
						break;
					}
				}
			}
			t = 0.3f;
		}

		if (_input->getInput(SDLK_EQUALS) && !_filter.getCollection().empty() && t <= 0.0f)
		{
			auto &collection = _filter.getCollection();

			if (collection.size() == 1)
			{
				_name = std::begin(collection)->get()->getComponent<Component::EntityPlacable>()->name;
			}
			else
			{
				for (auto it = std::begin(collection); it != std::end(collection); ++it)
				{
					auto c = it->get()->getComponent<Component::EntityPlacable>();
					if (c->name != _name)
						continue;
					auto cit = it;
					++cit;
					if (cit != std::end(collection))
					{
						_name = cit->get()->getComponent<Component::EntityPlacable>()->name;
						break;
					}
					else
					{
						_name = ((std::begin(collection)))->get()->getComponent<Component::EntityPlacable>()->name;
						break;
					}
				}
			}
			t = 0.3f;
		}
		if (t > 0.0f)
			t -= (float)time;
		_scene.lock()->getInstance<FontManager>()->draw2DString("Current entity : " + _name, "myFont", 30, glm::ivec2(10, 100), glm::vec4(1), "2DText");

		Entity _entity = getEntity();
		if (!_entity.get())
			return;

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


		if (_input->getInput(SDLK_u))
		{
			_entity->setLocalTransform(glm::translate(_entity->getLocalTransform(), glm::vec3(0,0.2,0)), true);
		}

		if (_input->getInput(SDLK_i))
		{
			_entity->setLocalTransform(glm::translate(_entity->getLocalTransform(), glm::vec3(0, -0.2, 0)), true);
		}

		if (_input->getInput(SDLK_j))
		{
			_entity->setLocalTransform(glm::translate(_entity->getLocalTransform(), glm::vec3(0.2, 0, 0)), true);
		}

		if (_input->getInput(SDLK_k))
		{
			_entity->setLocalTransform(glm::translate(_entity->getLocalTransform(), glm::vec3(-0.2, 0, 0)), true);
		}

		if (_input->getInput(SDLK_m))
		{
			_entity->setLocalTransform(glm::translate(_entity->getLocalTransform(), glm::vec3(0, 0, 0.2)), true);
		}

		if (_input->getInput(SDLK_COMMA))
		{
			_entity->setLocalTransform(glm::translate(_entity->getLocalTransform(), glm::vec3(0, 0, -0.2)), true);
		}

		if (_input->getInput(SDLK_t))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), -1.0f, glm::vec3(0, 1, 0)), false);
		}

		if (_input->getInput(SDLK_y))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), 1.0f, glm::vec3(0, 1, 0)), false);
		}

		if (_input->getInput(SDLK_b))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), -1.0f, glm::vec3(1, 0, 0)), true);
		}

		if (_input->getInput(SDLK_n))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), 1.0f, glm::vec3(1, 0, 0)), true);
		}

		if (_input->getInput(SDLK_g))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), -1.0f, glm::vec3(0, 0, 1)), true);
		}

		if (_input->getInput(SDLK_h))
		{
			_entity->setLocalTransform(glm::rotate(_entity->getLocalTransform(), 1.0f, glm::vec3(0, 0, 1)), true);
		}

		if (_input->getInput(SDLK_PERIOD))
		{
			_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(0.95f)), true);
		}

		if (_input->getInput(SDLK_SLASH))
		{
			_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1.05f)), true);
		}

		//if (_input->getInput(SDLK_u))
		//{
		//	_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(0.95f,1,1)), true);
		//}

		//if (_input->getInput(SDLK_i))
		//{
		//	_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1.05f,1,1)), true);
		//}

		//if (_input->getInput(SDLK_j))
		//{
		//	_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1, 0.95f,1)), true);
		//}

		//if (_input->getInput(SDLK_k))
		//{
		//	_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1, 1.05f,1)), true);
		//}

		//if (_input->getInput(SDLK_m))
		//{
		//	_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1, 1, 0.95f)), true);
		//}

		//if (_input->getInput(SDLK_COMMA))
		//{
		//	_entity->setLocalTransform(glm::scale(_entity->getLocalTransform(), glm::vec3(1, 1, 1.05f)), true);
		//}

	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::EntityPlacable>();
		return true;
	}
};