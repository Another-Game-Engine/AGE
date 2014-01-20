#ifndef		__TRACKING_COMPONENT_HH_
#define		__TRACKING_COMPONENT_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>

namespace Component
{
	class TrackingCamera : public Component::ComponentBase<TrackingCamera>
	{
	public:
		TrackingCamera() :
			Component::ComponentBase<Component::TrackingCamera>(),
			dist(0.0f)
//			,toLook()
		{}
		virtual ~TrackingCamera(void)
		{}
		void init(Entity _toLook, glm::vec3 _dist)
		{
			toLook = _toLook;
			dist = _dist;
		}
		void reset()
		{}
public:
	glm::vec3               dist;
	Entity                  toLook;
	private:
		TrackingCamera(TrackingCamera const &);
		TrackingCamera &operator=(TrackingCamera const &);
	};

}

#endif //!__TRACKING_COMPONENT_HH_