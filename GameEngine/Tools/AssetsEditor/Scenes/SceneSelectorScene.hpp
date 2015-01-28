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

		SceneSelectorScene(std::weak_ptr<AGE::Engine> engine);

		virtual ~SceneSelectorScene(void);
		virtual bool userStart();
		virtual bool userUpdate(double time);
	};
}