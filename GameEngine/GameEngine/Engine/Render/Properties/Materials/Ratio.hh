# pragma once

# include <Render/Properties/AProperty.hh>
# include <string>

namespace AGE
{
	class Ratio : public AProperty
	{
	public:
		Ratio(std::string const &add_name);
		Ratio(Ratio &&move);

	private:
		virtual void _update(std::shared_ptr<Program> const &p) override final;

	public:
		float get();
		Ratio &set(float ratio);

	private:
		float _ratio;
	};
}
