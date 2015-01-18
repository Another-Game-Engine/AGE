#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Core/AScene.hh>

namespace AGE
{
	namespace Component
	{
		struct TrackBall : public Component::ComponentBase < TrackBall >
		{
			TrackBall();
			virtual ~TrackBall(void);
			void init(AScene *, const Entity &_toLook, float _dist, float rotatingSpeed, float zoomingSpeed);
			virtual void reset(AScene *);

			//////
			////
			// Serialization

			template <typename Archive>
			void save(Archive &ar) const
			{
				ar(CEREAL_NVP(dist), CEREAL_NVP(rotateSpeed), CEREAL_NVP(zoomSpeed), CEREAL_NVP(angles));
				// @CESAR TODO
				//std::size_t toLookId = _entity.get()->getScene().lock()->registrarSerializedEntity(toLook.getId());
				//ar(CEREAL_NVP(toLookId));
			}

			template <typename Archive>
			void load(Archive &ar)
			{
				ar(dist, rotateSpeed, zoomSpeed, angles);
				std::size_t toLookId = 42;
				ar(toLookId);
				// @CESAR TODO
				//_entity->getScene().lock()->entityHandle(toLookId, &toLook);
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
}