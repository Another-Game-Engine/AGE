#pragma once

#include <Core/EntityFilter.hpp>

namespace AGE
{
	class	System
	{
	public:
		System(AScene *scene);
		virtual ~System();
		void update(float time);
		bool init();
		bool setActivation(bool tof);
		bool isActivated() const;
		inline const std::string &getName() const { return _name; }
	protected:
		AScene *_scene;
		std::string _name;
		bool _activated;
	
		virtual void updateBegin(float time) {}
		virtual void updateEnd(float time) {}
		virtual void mainUpdate(float time) {}
		virtual bool initialize() { return true; }
		virtual bool activate(){ return true; }
		virtual bool deactivate(){ return true; }
	};
}
