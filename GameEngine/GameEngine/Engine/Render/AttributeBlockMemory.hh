#pragma once

# include <vector>
# include <memory>
# include <Render/Attribute.hh>

template <typename type_t> class Key;

class Data;
class Buffer;

class AttributeBlockMemory
{
public:
	Key<Data> addElement(std::shared_ptr<Data> const &data);
	AttributeBlockMemory &deleteElement(Key<Data> const &data);
	AttributeBlockMemory &setElement(Key<Data> const &data);

private:
	Attribute _type;
	size_t _size;
	std::vector<std::shared_ptr<Data>> _elements;
	std::unique_ptr<Buffer> _buffer;
};
