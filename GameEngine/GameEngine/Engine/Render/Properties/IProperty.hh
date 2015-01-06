#pragma once

# include <string>

class IProperty
{
public:
	virtual ~IProperty() {};
	virtual IProperty &update() = 0;
	virtual std::string const &name() const = 0;
};