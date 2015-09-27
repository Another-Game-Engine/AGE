#pragma once

#include "Core/Ascene.hh"

namespace AGE
{
	class AScene;
	class Engine;

	class IMenuInheritance : public AScene
	{
	public:
		IMenuInheritance(AGE::Engine *engine)
			: AScene(engine)
		{}
		virtual void updateMenu() = 0;
		virtual ~IMenuInheritance(){}
		bool _displayWindow = false;
	};
}