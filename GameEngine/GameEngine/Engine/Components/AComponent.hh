#ifndef		ACOMPONENT_HH_
#define		ACOMPONENT_HH_

#include <string>

class	AEntity;

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
	AEntity				*_father;

protected:
	const ComponentType	_type;

public:
	AComponent(std::string const &name);
	virtual ~AComponent(void);

	ComponentType	getType() const;

	virtual void	start() = 0;
	virtual void	update() = 0;
	virtual void	stop() = 0;

	void			setFather(AEntity *father);
	AEntity			*getFather() const;

	std::string const &getName() const;
};

}

#endif