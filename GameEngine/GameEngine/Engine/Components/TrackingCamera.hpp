#pragma once

#include "Component.hh"
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>

namespace Component
{
	struct TrackingCamera : public Component::ComponentBase<TrackingCamera>
	{
		TrackingCamera();
		virtual ~TrackingCamera(void);
		void init(Entity _toLook, glm::vec3 _dist);
		virtual void reset();

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