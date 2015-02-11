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

		void serializeComponentsToJson(cereal::JSONOutputArchive &ar) const
		{
			for (auto &e : components)
			{
				ComponentRegistrar::getInstance().serializeJson(e, ar);
				//e->saveComponent(ar);
			}
		}

		void serializeComponentsToBinary(cereal::PortableBinaryOutputArchive &ar) const
		{
			//for (auto &e : components)
			//	e->serializeBaseToBinary(ar);
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