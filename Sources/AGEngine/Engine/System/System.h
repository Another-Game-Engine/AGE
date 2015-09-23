#pragma once

#include <Core/EntityFilter.hpp>

namespace AGE
{
	typedef std::uint32_t SystemType;

	class	SystemBase
	{
	public:
		SystemBase(AScene *scene, const SystemType typeId);
		virtual ~SystemBase();
		void update(float time);
		bool init();
		virtual void finalize(void);
		bool setActivation(bool tof);
		bool isActivated() const;
		inline const std::string &getName() const { return _name; }
		inline const SystemType getTypeId() const { return _typeId; }
	protected:
		AScene *_scene;
		std::string _name;
		bool _activated;
		const SystemType _typeId = -1;
		static SystemType _typeCounter;
	
		virtual void updateBegin(float time);
		virtual void updateEnd(float time);
		virtual void mainUpdate(float time);
		virtual bool initialize();
		virtual bool activate();
		virtual bool deactivate();
	};

	template <typename Type>
	struct System : public SystemBase
	{
		System(AScene *scene)
			: SystemBase(scene, getTypeId())
		{
		}
		static SystemType getTypeId()
		{
			static SystemType type = _typeCounter++;
			return type;
		}
	};
}
