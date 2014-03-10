#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/SpriteComponent.hh>
#include <Entities/EntityData.hh>

class SpriteSystem : public System
{
public:
	SpriteSystem(AScene *scene)
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
		for (auto e : _filter.getCollection())
		{
			auto s = e->getComponent<Component::Sprite>();
			s->shader->use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, s->animation->getTexture()->getId());
 			s->animation->update(s->index);
			s->animation->draw(s->index);
			//glm::vec3 force = e->getComponent<Component::RotationForce>()->getForce();
			//e->rotate(force * glm::vec3(t));
			//e->setLocalTransform(glm::rotate(e->getLocalTransform(), force.x * t, glm::vec3(1, 0, 0)));
			//e->setLocalTransform(glm::rotate(e->getLocalTransform(), force.y * t, glm::vec3(0, 1, 0)));
			//e->setLocalTransform(glm::rotate(e->getLocalTransform(), force.z * t, glm::vec3(0, 0, 1)));
		}
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::Sprite>();
	}
};