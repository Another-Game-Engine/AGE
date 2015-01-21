#pragma once

# include <string>
# include <memory>
# include <vector>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class IProperty
{
public:
	virtual ~IProperty() {};
	virtual IProperty &update(std::shared_ptr<Program> const &p) = 0;
	virtual std::shared_ptr<IProgramResources> const & get_resource(std::shared_ptr<Program> const &p) = 0;
	virtual std::string const &name() const = 0;
	virtual IProperty &set_program(std::vector<std::shared_ptr<Program>> const &programs) = 0;
};

typedef IProperty Property;