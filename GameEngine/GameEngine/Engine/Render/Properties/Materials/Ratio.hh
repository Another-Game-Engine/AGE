# pragma once

# include <Render/Properties/AProperty.hh>
# include <string>

class Ratio : public AProperty
{
public:
	Ratio(std::string const &add_name);
	Ratio(Ratio &&move);

public:
	virtual IProperty &update(std::shared_ptr<Program> const &p) override final;

public:
	float get() const;
	Ratio &set(float ratio);

private:
	float _ratio;
};