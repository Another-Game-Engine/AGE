#ifndef   __COLLISION_HPP__
# define  __COLLISION_HPP__

#include <Components/Component.hh>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/base_class.hpp>

namespace Component
{
	struct Collision : public ComponentBase<Collision>
	{
		Collision()
			: ComponentBase<Collision>()
			, force(0)
		{
				
		}

		virtual ~Collision()
		{
		}

		void init()
		{
			force = 0;
		}

		virtual void reset()
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

		virtual Collision &operator=(const Collision &other)
		{
			force = other.force;
			collisions = other.collisions;
			return *this;
		}


		// !Serialization
		////
		//////
		float force;
		std::set<Entity> collisions;
	};
}

#endif    //__COLLISION_HPP__