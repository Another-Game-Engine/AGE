#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>

namespace AGE
{
	class SceneSelectorScene : public AScene
	{
	public:
		static const std::string Name;

		SceneSelectorScene(AGE::Engine *engine);

		virtual ~SceneSelectorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);
	};
}