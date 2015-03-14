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

		WorldEditorScene(Engine *engine);

		virtual ~WorldEditorScene(void);
		virtual bool userStart();
		virtual bool userUpdateBegin(float time);
		virtual bool userUpdateEnd(float time);
	};
}