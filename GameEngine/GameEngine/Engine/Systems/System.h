#ifndef		__SYSTEM_H__
# define	__SYSTEM_H__

#include    <set>
#include	<Utils/Barcode.h>
#include    <Utils/PubSub.hpp>
#include    <Entities/EntityData.hh>
#include    <Core/EntityFilter.hpp>
//#include    <Entities/Entity.hh>

class	System : public PubSub
{
public:
	System(std::weak_ptr<AScene> scene);
	virtual ~System();
	void update(double time);
	void init();
	inline const std::string &getName() const {return _name; }
protected:
	std::weak_ptr<AScene> _scene;
	std::string _name;
private:
	virtual void updateBegin(double time) = 0;
	virtual void updateEnd(double time) = 0;
	virtual void mainUpdate(double time) = 0;
	virtual void initialize() = 0;
};

#endif		//__SYSTEM_H__