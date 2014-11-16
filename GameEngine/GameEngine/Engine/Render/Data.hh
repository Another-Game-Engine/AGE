#pragma once

# include <vector>
# include <Render/Attribute.hh>

class Data
{
public:
	Data(size_t nbrElement, Attribute type);

public:
	bool operator==(Data const &data);

private:
	Attribute _type;
	std::vector<uint8_t> _data;
};