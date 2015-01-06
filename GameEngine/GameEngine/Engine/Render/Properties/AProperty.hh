#pragma once

# include <Render/Properties/IProperty.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/Program.hh>

class AProperty : public IProperty
{
public:
	AProperty(std::string &&name, std::vector<std::shared_ptr<Program>> const &programs);
	AProperty(AProperty &&move);

public:
	virtual std::string const &name();

private:
	std::string _name;
	std::vector<IProgramResources> _resources;
};