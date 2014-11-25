#pragma once

# include <vector>
# include <Render/Attribute.hh>
# include <glm/glm.hpp>

class Data
{
public:
	template <typename type_t> Data(std::vector<type_t> const &data, Attribute type);

public:
	operator Attribute() const;
	operator size_t() const;
	size_t size() const;
	glm::vec2 const &range() const;
	Data &range(glm::vec2 const &r);
	void const *data() const;

private:
	Attribute _type;
	std::vector<uint8_t> _data;
	glm::vec2 _range;
};

template <typename type_t>
Data::Data(std::vector<type_t> const &data, Attribute type) :
_type(type),
_range(glm::vec2(0.f))
{
	_data.resize(sizeof(type_t)* data.size());
	for (size_t index = 0; index < _data.size(); ++index) {
		_data[index] = ((uint8_t *)data.data())[index];
	}
}
