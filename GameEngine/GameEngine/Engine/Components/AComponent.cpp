#include "AComponent.hh"

#include "Entities/Entity.hh"

namespace	Components
{

AComponent::AComponent(std::string const &name) :
	_name(name),
	_type(UNKNOWN),
	_father(NULL)
{
}

AComponent::~AComponent(void)
{
	stop();
}

AComponent::ComponentType	AComponent::getType() const
{
	return (_type);
}

void						AComponent::setFather(Entity *father)
{
	if (_father != NULL)
	{
		_father->removeComponent(_name);
		stop();
	}
	_father = father;
	start();
}

Entity						*AComponent::getFather() const
{
	return (_father);
}

std::string const			&AComponent::getName() const
{
	return (_name);
}

}