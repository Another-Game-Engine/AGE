#pragma once

# include <vector>
# include <memory>
# include <utility>
# include <Render/GeometryManagement/Attribute.hh>
# include <Render/Buffer/IBuffer.hh>

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
	operator Attribute() const;
	bool operator!=(Attribute attribute) const;
	bool operator==(Attribute attribute) const;
	AttributeBlockMemory &add(std::shared_ptr<Data> const &data);
	AttributeBlockMemory &update();
	IBuffer const &buffer() const;

private:
	AttributeBlockMemory(Attribute type, size_t size, std::unique_ptr<Buffer> &&buffer);

private:
	Attribute _type;
	std::vector<std::weak_ptr<Data>> _elements;
	std::unique_ptr<IBuffer> _buffer;
	bool _update;
};