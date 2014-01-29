#ifndef		__TRACKING_COMPONENT_HH_
#define		__TRACKING_COMPONENT_HH_

#include "Component.hh"
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>

namespace Component
{
	struct TrackingCamera : public Component::ComponentBase<TrackingCamera>
	{
		TrackingCamera() :
			Component::ComponentBase<Component::TrackingCamera>(),
			dist(0.0f)
		{}
		virtual ~TrackingCamera(void)
		{}
		void init(Entity _toLook, glm::vec3 _dist)
		{
			toLook = _toLook;
			dist = _dist;
		}
		virtual void reset()
		{}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new TrackingCamera();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(dist));
		}

		// !Serialization
		////
		//////

		glm::vec3               dist;
		Entity                  toLook;
	private:
		TrackingCamera(TrackingCamera const &);
		TrackingCamera &operator=(TrackingCamera const &);
	};

}

#endif //!__TRACKING_COMPONENT_HH_