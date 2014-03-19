#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/SpriteComponent.hh>
#include <Entities/EntityData.hh>
#include <Context/IRenderContext.hh>

class SpriteSystem : public System
{
public:
	SpriteSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _filter(scene)
	{
		_name = "sprite_system";
	}
	virtual ~SpriteSystem(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		glDepthFunc(GL_ALWAYS);

		auto screen = _scene.lock()->getInstance<IRenderContext>()->getScreenSize();
		glm::mat4 Projection = glm::mat4(1);
		Projection *= glm::ortho(0.0f, (float)screen.x, (float)screen.y, 0.0f, -1.0f, 1.0f);
		//auto glyphWidth = 0.0f;
		//float lastX = position.x;

		for (auto e : _filter.getCollection())
		{
			auto s = e->getComponent<Component::Sprite>();
			s->shader->use();
			auto transformationID = glGetUniformLocation(s->shader->getId(), "transformation");
			glUniformMatrix4fv(glGetUniformLocation(s->shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
			glUniform1i(glGetUniformLocation(s->shader->getId(), "fTexture0"), 0);
			glUniform4f(glGetUniformLocation(s->shader->getId(), "color"), 1, 1, 1, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, s->animation->getTexture()->getId());
			glUniformMatrix4fv(transformationID, 1, GL_FALSE, glm::value_ptr(e->getLocalTransform()));
			s->animation->update(s->index);
			s->animation->draw(s->index);
		}

		glDepthFunc(GL_LESS);
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::Sprite>();
	}
};