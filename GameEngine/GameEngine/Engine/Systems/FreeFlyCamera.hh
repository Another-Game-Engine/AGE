#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <vector>

namespace AGE
{
	class FreeFlyCamera : public System
	{
	public:
		FreeFlyCamera() = delete;
		FreeFlyCamera(AScene *scene);
		~FreeFlyCamera() = default;

	private:
		EntityFilter _cameras;
		std::vector<glm::vec3> _cameraAngles;

		virtual bool initialize();
		virtual void updateBegin(double time);
		virtual void mainUpdate(double time);
		virtual void updateEnd(double time);
	};
}
