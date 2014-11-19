#pragma once

# include <vector>
# include <memory>
# include <Render/Attribute.hh>
# include <Render/Buffer.hh>
# include <type_traits>

template <typename type_t> class Key;

class Data;
class Buffer;

class AttributeBlockMemory
{
public:
	AttributeBlockMemory();
	AttributeBlockMemory(Attribute type);
	AttributeBlockMemory(AttributeBlockMemory const &copy) = delete;
	AttributeBlockMemory(AttributeBlockMemory &&move);
	AttributeBlockMemory &operator=(AttributeBlockMemory const &a) = delete;
	AttributeBlockMemory &operator=(AttributeBlockMemory &&a);


public:
	AttributeBlockMemory &addElement(std::weak_ptr<Data> const &data);

private:
	Attribute _type;
	size_t _size;
	std::vector<std::weak_ptr<Data>> _elements;
	std::unique_ptr<Buffer> _buffer;

private:
	std::unique_ptr<Buffer> generatePtr();
};