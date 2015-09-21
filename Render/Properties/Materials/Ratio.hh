# pragma once

# include <Render/Properties/AProperty.hh>
# include <string>

namespace AGE
{
	class Vec1;

	class Ratio : public AProperty
	{
	public:
		Ratio(std::string const &add_name);
		Ratio(Ratio &&move);
		PROPERTY_UPDATE_FUNCTION(Ratio, Vec1);
		PROPERTY_INSTANCIED_UPDATE_FUNCTION(Ratio, Vec1);
	public:
		float get();
		Ratio &set(float ratio);

	private:
		float _ratio;
	};
}
