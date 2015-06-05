#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include "IMenuInheritrance.hpp"

namespace AGE
{
	class MainMenuScene : public AScene
	{
		enum ToolType
		{
			WorldEditor = 0
			, MaterialEditor = 1
			, AssetsEditor = 2
			, TOOLS_NUMBER
		};

		std::string _toolsName[TOOLS_NUMBER];
		IMenuInheritance* _scenes[TOOLS_NUMBER];

		bool _exitApp;

	public:
		static const std::string Name;


		MainMenuScene(AGE::Engine *engine);

		virtual ~MainMenuScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);
	};
}