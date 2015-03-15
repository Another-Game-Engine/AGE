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
	private:
		virtual void updateBegin(float time) = 0;
		virtual void updateEnd(float time) = 0;
		virtual void mainUpdate(float time) = 0;
		virtual bool initialize() = 0;
		virtual bool activate(){ return true; };
		virtual bool deactivate(){ return true; };
	};
}
