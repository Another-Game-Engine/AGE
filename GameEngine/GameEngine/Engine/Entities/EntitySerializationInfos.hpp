#include "Entity.hh"
#include <Components/Component.hh>
#include <vector>
#include <cereal/types/vector.hpp>
#include <Entities/EntityData.hh>

namespace AGE
{
	struct EntitySerializationInfos
	{
		AGE::Link link;
		ENTITY_FLAGS flags;
		std::vector <std::size_t> componentsHash;
		std::vector <ComponentBase*> components;

		EntitySerializationInfos(const EntityData& e)
			: link(e.getLink())
			, flags(e.getEntity().getFlags())
		{}

		template < typename Archive >
		void save(Archive &ar) const
		{
			auto componentSize = components.size();
			ar(
				cereal::make_nvp("link", link)
				, cereal::make_nvp("flags", flags)
				, cereal::make_nvp("components_hash", componentsHash)
				);
		}

		template < typename Archive >
		void serializeComponents(Archive &ar, AScene *scene) const
		{
			// @ECS TODO

			//for (auto &e : components)
			//	e->serializeBase(ar, scene);
		}

		template < typename Archive >
		void load(Archive &ar)
		{
			std::size_t cptNbr = 0;
			ar(
				link
				, flags
				, componentsHash
				);
		}
	};
}