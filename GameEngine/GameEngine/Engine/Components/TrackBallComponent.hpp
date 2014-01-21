#ifndef		__TRACKBALL_COMPONENT_HH_
#define		__TRACKBALL_COMPONENT_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>

namespace Component
{
	struct TrackBall : public Component::ComponentBase<TrackBall>
	{
		TrackBall() :
			Component::ComponentBase<Component::TrackBall>(),
			dist(0.0f),
			rotateSpeed(0.0f),
			zoomSpeed(0.0f),
			angles(0, 0)
		{}
		virtual ~TrackBall(void)
		{}
		void init(Entity _toLook, float _dist, float rotatingSpeed, float zoomingSpeed)
		{
			toLook = _toLook;
			dist = _dist;
			rotateSpeed = rotatingSpeed;
			zoomSpeed = zoomingSpeed;
		}
		virtual void reset()
		{}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar)
		{
			auto res = new TrackBall();
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(dist), CEREAL_NVP(rotateSpeed), CEREAL_NVP(zoomSpeed), CEREAL_NVP(angles));
		}

		// !Serialization
		////
		//////

		float					dist;
		float					rotateSpeed, zoomSpeed;
		glm::vec2				angles;
		Entity                  toLook;
	private:
		TrackBall(TrackBall const &);
		TrackBall &operator=(TrackBall const &);
	};

}

#endif //!__TRACKBALL_COMPONENT_HH_