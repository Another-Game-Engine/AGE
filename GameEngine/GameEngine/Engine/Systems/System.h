#ifndef		__SYSTEM_H__
# define	__SYSTEM_H__

#include	<iostream>
#include	<memory>
#include    <Utils/PubSub.hpp>

class AScene;

class	System : public PubSub
{
public:
	System(std::weak_ptr<AScene> &&scene);
	virtual ~System();
	void update(double time);
	bool init();
	bool setActivation(bool tof);
	bool isActivated() const;
	inline const std::string &getName() const {return _name; }
protected:
	std::weak_ptr<AScene> _scene;
	std::string _name;
	bool _activated;
private:
	virtual void updateBegin(double time) = 0;
	virtual void updateEnd(double time) = 0;
	virtual void mainUpdate(double time) = 0;
	virtual bool initialize() = 0;
	virtual bool activate(){ return true; };
	virtual bool deactivate(){ return true; };
};

#endif		//__SYSTEM_H__