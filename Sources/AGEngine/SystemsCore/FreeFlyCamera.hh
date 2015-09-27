#pragma once

#include <vector>

#include <System/System.h>

#include <Core/EntityFilter.hpp>

#include <glm/fwd.hpp>

namespace AGE
{
	struct Link;

	class FreeFlyCamera : public System<FreeFlyCamera>
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

		void _handleKeyboard(float time, Link &camLink, size_t camIdx);
		void _handleMouse(float time, Link &camLink, size_t camIdx);
		void _handleController(float time, Link &camLink, size_t camIdx);
	};
}
