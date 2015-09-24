#pragma once

#include <memory>
#include "IMenuInheritrance.hpp"

namespace AGE
{
	class Engine;

	class WorldEditorScene : public IMenuInheritance
	{
	public:
		static const std::string Name;

		WorldEditorScene(Engine *engine);

		virtual ~WorldEditorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);
		virtual void updateMenu();
	};
}