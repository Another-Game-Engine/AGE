#pragma once

# include <string>
# include <memory>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class IProperty
{
public:
	virtual ~IProperty() {};
	virtual IProperty &update(std::shared_ptr<Program> const &p) = 0;
	virtual IProgramResources *get_resource(std::shared_ptr<Program> const &p) = 0;
	virtual std::string const &name() const = 0;
};

typedef IProperty Property;