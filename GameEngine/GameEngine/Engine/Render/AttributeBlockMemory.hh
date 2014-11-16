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
	Key<Data> addElement(Data const &data);
	Key<Data> addElement(Data &&data);
	AttributeBlockMemory &deleteElement(Key<Data> const &data);
	AttributeBlockMemory &setElement(Key<Data> const &data);

private:
	Attribute _type;
	size_t _size;
	std::vector<Data> _elements;
	std::unique_ptr<Buffer> _buffer;
};
