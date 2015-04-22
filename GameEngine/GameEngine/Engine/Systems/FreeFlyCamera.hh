#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <vector>
#include <Engine/Components/FreeFlyComponent.hh>

namespace AGE
{
	struct Link;

	class FreeFlyCamera : public System
	{
	public:
		FreeFlyCamera() = delete;
		FreeFlyCamera(AScene *scene);
		~FreeFlyCamera() = default;

	private:
		EntityFilter _cameras;
		std::vector<glm::vec2> _cameraAngles;

		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);

		void _handleKeyboard(float time, Link &camLink, size_t camIdx, FreeFlyComponent const *component);
		void _handleMouse(float time, Link &camLink, size_t camIdx, FreeFlyComponent const *component);
		void _handleController(float time, Link &camLink, size_t camIdx, FreeFlyComponent const *component);
	};
}
