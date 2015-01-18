#pragma once

#include <Components/Component.hh>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/base_class.hpp>

namespace AGE
{
	namespace Component
	{
		struct Collision : public ComponentBase < Collision >
		{
			Collision()
				: ComponentBase<Collision>()
				, force(0)
			{

			}

			virtual ~Collision()
			{
			}

			Collision &operator=(Collision &&other)
			{
				force = std::move(other.force);
				collisions = std::move(other.collisions);
				return *this;
			}

			Collision(Collision &&other)
				: ComponentBase<Collision>(std::move(other))
			{
				force = std::move(other.force);
				collisions = std::move(other.collisions);
			}

			void init(AScene *)
			{
				force = 0;
			}

			virtual void reset(AScene *)
			{
				collisions.clear();
			}

			void addCollision(const Entity &entity)
			{
				collisions.insert(entity);
			}

			std::set<Entity> &getCollisions()
			{
				return collisions;
			}

			void clear()
			{
				collisions.clear();
			}

			//////
			////
			// Serialization

			template <typename Archive>
			void save(Archive &ar) const
			{
				std::set<std::size_t> entityIds;
				for (auto e : collisions)
				{
					//@CESAR TODO
					/*entityIds.insert(_entity.get()->getScene().lock()->registrarSerializedEntity(e.getId()));*/
				}
				ar(cereal::make_nvp("Collisions", entityIds));
			}

			template <typename Archive>
			void load(Archive &ar)
			{
				std::set<std::size_t> entityIds;
				ar(entityIds);
				//@CESAR TODO
				//for (auto e : entityIds)
				//	collisions.insert(Entity(e));
				for (auto it = std::begin(collisions); it != std::end(collisions); ++it)
				{
					//@CESAR TODO
					//Entity *e = const_cast<Entity *>(&(*it));
					//_entity->getScene().lock()->entityHandle(it->getId(), e);
				}
			}

			// !Serialization
			////
			//////
			float force;
			std::set<Entity> collisions;

		private:
			Collision &operator=(Collision const &o);
			Collision(Collision const &o);
		};
	}
}
