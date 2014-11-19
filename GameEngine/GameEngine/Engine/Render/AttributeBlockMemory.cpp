#include <Render/AttributeBlockMemory.hh>
#include <Render/Key.hh>
#include <Render/Data.hh>
#include <utility>

AttributeBlockMemory::AttributeBlockMemory():
_type(Attribute::Position),
_size(0),
_buffer(nullptr)
{

}

AttributeBlockMemory::AttributeBlockMemory(Attribute type):
_type(type),
_size(0),
_buffer(generatePtr())
{
}

AttributeBlockMemory::AttributeBlockMemory(AttributeBlockMemory &&move):
_type(move._type),
_size(move._size),
_buffer(std::move(move._buffer))
{
}

AttributeBlockMemory &AttributeBlockMemory::operator=(AttributeBlockMemory &&a)
{
	_type = a._type;
	_size = a._size;
	_buffer = std::move(a._buffer);
	return (*this);
}

AttributeBlockMemory &AttributeBlockMemory::addElement(std::weak_ptr<Data> const &data)
{
	for (auto &element : _elements) 
	{
		if (!element.lock())
		{
			element = data;
			return (*this);
		}
	}
	_elements.emplace_back(data);
	auto &element = _elements.back();
	_size += element.lock()->getSize();
	return (*this);
}

std::unique_ptr<Buffer> AttributeBlockMemory::generatePtr()
{
	if (_type == Attribute::Indices)
	{
		return (std::make_unique<IndexBuffer>());
	}
	else
	{
		return (std::make_unique<VertexBuffer>());
	}
}
