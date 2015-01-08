#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>

namespace AGE
{
	class AssetsEditorScene : public AScene
	{
	public:
		AssetsEditorScene(std::weak_ptr<AGE::Engine> engine);

		virtual ~AssetsEditorScene(void);
		virtual bool userStart();
		virtual bool userUpdate(double time);
	};
}