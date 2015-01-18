#include <Components/TrackingCamera.hpp>

namespace AGE
{
	using namespace Component;

	TrackingCamera::TrackingCamera() :
		Component::ComponentBase<Component::TrackingCamera>(),
		dist(0.0f)
	{}

	TrackingCamera::~TrackingCamera(void)
	{}

	void TrackingCamera::init(AScene *, const Entity &_toLook, glm::vec3 _dist)
	{
		toLook = _toLook;
		dist = _dist;
	}

	void TrackingCamera::reset(AScene *)
	{}
}