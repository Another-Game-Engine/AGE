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
		virtual bool userStart();
		virtual bool userUpdateBegin(float time);
		virtual bool userUpdateEnd(float time);
	};
}