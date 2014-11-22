#pragma once

# include <queue>
# include <vector>
# include <memory>
# include <utility>
# include <Render/Attribute.hh>
# include <Render/Buffer.hh>

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
	bool operator!=(Data const &data) const;
	bool operator==(Data const &data) const;
	AttributeBlockMemory &handle(std::shared_ptr<Data> const &data);
	AttributeBlockMemory &update();

private:
	AttributeBlockMemory(Attribute type, size_t size, std::unique_ptr<Buffer> &&buffer);

private:
	Attribute _type;
	std::vector<std::weak_ptr<Data>> _elements;
	std::unique_ptr<Buffer> _buffer;
	bool _update;
};