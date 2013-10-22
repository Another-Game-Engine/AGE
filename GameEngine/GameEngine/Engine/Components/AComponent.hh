#ifndef		ACOMPONENT_HH_
#define		ACOMPONENT_HH_

#include <string>

class	Entity;

namespace	Components
{

class AComponent
{
public:
	enum	ComponentType
	{
		RENDERER,
		BEHAVIOR,
		UNKNOWN
	};

private:
	const std::string	_name;
	Entity				*_father;

	AComponent();

protected:
	const ComponentType	_type;

public:
	AComponent(std::string const &name);
	virtual ~AComponent(void);

	ComponentType	getType() const;

	virtual void	start() {}
	virtual void	update() {}
	virtual void	stop() {}

	void			setFather(Entity *father);
	Entity			*getFather() const;

	std::string const &getName() const;
};

}

#endif