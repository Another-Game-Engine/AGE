#pragma once

namespace AGE
{
	class IMenuInheritance
	{
	public:
		virtual void updateMenu() = 0;
		virtual ~IMenuInheritance(){}
	};
}