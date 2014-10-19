#include "Entity.hh"
#include <Components/Component.hh>
#include <cereal/types/vector.hpp>
#include <vector>

struct EntitySerializationInfos
{
	AGE::Link link;
	Barcode barcode;
	ENTITY_FLAGS flags;
	std::vector <Component::Base*> components;

	EntitySerializationInfos(const EntityData& e)
		: link(e.getLink())
		, barcode(e.getBarcode())
		, flags(e.getEntity().getFlags())
	{}

	template < typename Archive >
	void save(Archive &ar) const
	{
		ar(
			cereal::make_nvp("link", link)
			, cereal::make_nvp("barcode", barcode)
			, cereal::make_nvp("flags", flags)
			, cereal::make_nvp("components_number", components.size())
			);
		for (auto &e : components)
			e->serializeBase(ar);
	}

	template < typename Archive >
	void load(Archive &ar)
	{
		std::size_t cptNbr = 0;
		ar(
			link
			, barcode
			, flags
			, cptNbr
			);
	}
};