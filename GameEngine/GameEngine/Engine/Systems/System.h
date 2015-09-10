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
		virtual void finalize(void);
		bool setActivation(bool tof);
		bool isActivated() const;
		inline const std::string &getName() const { return _name; }
	protected:
		AScene *_scene;
		std::string _name;
		bool _activated;
	
		virtual void updateBegin(float time);
		virtual void updateEnd(float time);
		virtual void mainUpdate(float time);
		virtual bool initialize();
		virtual bool activate();
		virtual bool deactivate();
	};
}
