#include "AComponent.hh"

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
}

AComponent::ComponentType	AComponent::getType() const
{
	return (_type);
}

void						AComponent::setFather(AEntity *father)
{
	if (_father != NULL)
	{
		_father->removeComponent(_name);
		stop();
	}
	_father = father;
	start();
}

AEntity						*AComponent::getFather() const
{
	return (_father);
}

}