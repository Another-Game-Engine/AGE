#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>

namespace AGE
{
	class WorldEditorScene : public AScene
	{
	public:
		static const std::string Name;

		WorldEditorScene(std::weak_ptr<AGE::Engine> engine);

		virtual ~WorldEditorScene(void);
		virtual bool userStart();
		virtual bool userUpdateBegin(double time);
		virtual bool userUpdateEnd(double time);
	};
}