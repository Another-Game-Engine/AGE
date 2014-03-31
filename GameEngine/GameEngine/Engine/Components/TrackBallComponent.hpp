#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Core/AScene.hh>

namespace Component
{
	struct TrackBall : public Component::ComponentBase<TrackBall>
	{
		TrackBall();
		virtual ~TrackBall(void);
		void init(const Entity &_toLook, float _dist, float rotatingSpeed, float zoomingSpeed);
		virtual void reset();

		//////
		////
		// Serialization

		template <typename Archive>
		void save(Archive &ar) const
		{
			ar(CEREAL_NVP(dist), CEREAL_NVP(rotateSpeed), CEREAL_NVP(zoomSpeed), CEREAL_NVP(angles));
			std::size_t toLookId = _entity.get()->getScene()->registrarSerializedEntity(toLook.getId());
			ar(CEREAL_NVP(toLookId));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(dist, rotateSpeed, zoomSpeed, angles);
			std::size_t toLookId = 42;
			ar(toLookId);
			_entity->getScene()->entityHandle(toLookId, &toLook);
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