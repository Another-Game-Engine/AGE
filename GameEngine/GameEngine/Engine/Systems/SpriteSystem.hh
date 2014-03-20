#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/SpriteComponent.hh>
#include <Entities/EntityData.hh>
#include <Context/IRenderContext.hh>
#include <Components/CameraComponent.hpp>

class SpriteSystem : public System
{
public:
	SpriteSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _filter(scene)
		, _cameras(scene)
	{
		_name = "sprite_system";
	}
	virtual ~SpriteSystem(){}
private:
	EntityFilter _filter;
	EntityFilter _cameras;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto c : _cameras.getCollection())
		{
			glm::mat4 Projection = c->getComponent<Component::CameraComponent>()->projection;
			auto View = c->getComponent<Component::CameraComponent>()->lookAtTransform;
			//auto glyphWidth = 0.0f;
			//float lastX = position.x;

			for (auto e : _filter.getCollection())
			{
				auto s = e->getComponent<Component::Sprite>();
				s->shader->use();
				auto transformationID = glGetUniformLocation(s->shader->getId(), "transformation");
				glUniformMatrix4fv(glGetUniformLocation(s->shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
				glUniformMatrix4fv(glGetUniformLocation(s->shader->getId(), "view"), 1, GL_FALSE, glm::value_ptr(View));
				glUniform1i(glGetUniformLocation(s->shader->getId(), "fTexture0"), 0);
				glUniform4f(glGetUniformLocation(s->shader->getId(), "color"), 1, 1, 1, 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, s->animation->getTexture()->getId());
				glUniformMatrix4fv(transformationID, 1, GL_FALSE, glm::value_ptr(e->getGlobalTransform()));
				s->animation->update(s->index);
				s->animation->draw(s->index);
			}
		}
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::Sprite>();
		_cameras.requireComponent<Component::CameraComponent>();
	}

};