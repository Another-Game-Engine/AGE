#pragma once

# include <Render/Properties/IProperty.hh>
# include <Render/Program.hh>
# include <utility>

class AProperty : public IProperty
{
protected:
	AProperty(std::string &&name);
	AProperty(std::string &&name, std::vector<std::shared_ptr<Program>> const &programs);
	AProperty(AProperty &&move);

public:
	virtual std::string const &name() const override final;
	virtual std::shared_ptr<IProgramResources> const & get_resource(std::shared_ptr<Program> const &p) override final;

private:
	std::string _name;
	std::vector<std::pair<std::shared_ptr<Program>, std::shared_ptr<IProgramResources>>> _resources;
};