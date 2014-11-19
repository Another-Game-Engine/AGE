#pragma once

# include <vector>
# include <Render/Attribute.hh>

class Data
{
public:
	template <typename type_t> Data(std::vector<type_t> const &data, Attribute type);

public:
	operator size_t();
	size_t getSize() const;
	void const *getData() const;

private:
	Attribute _type;
	std::vector<uint8_t> _data;
};

template <typename type_t>
Data::Data(std::vector<type_t> const &data, Attribute type) :
_type(type)
{
	_data.resize(sizeof(type_t)* data.size());
	for (size_t index = 0; index < _data.size(); ++index)
	{
		_data[index] = ((uint8_t *)data.data())[index];
	}
}
