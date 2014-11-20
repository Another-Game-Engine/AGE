#include <Render/AttributeBlockMemory.hh>
#include <Render/Key.hh>
#include <Render/Data.hh>
#include <utility>

AttributeBlockMemory::AttributeBlockMemory():
_type(Attribute::Position),
_size(0),
_buffer(nullptr),
_updateMajor(false),
_updateMinor(false)
{

}

AttributeBlockMemory::AttributeBlockMemory(Attribute type):
_type(type),
_size(0),
_buffer((_type == Attribute::Indices) ? std::unique_ptr<Buffer>(std::make_unique<IndexBuffer>()) : std::unique_ptr<Buffer>(std::make_unique<VertexBuffer>())),
_updateMajor(false),
_updateMinor(false)
{
}

AttributeBlockMemory::AttributeBlockMemory(AttributeBlockMemory &&move) :
_type(move._type),
_size(move._size),
_elements(std::move(move._elements)),
_buffer(std::move(move._buffer)),
_updateMajor(move._updateMajor),
_updateMinor(move._updateMinor)
{
}

AttributeBlockMemory &AttributeBlockMemory::operator=(AttributeBlockMemory &&a)
{
	_type = a._type;
	_size = a._size;
	_elements = std::move(a._elements);
	_buffer = std::move(a._buffer);
	_updateMajor = a._updateMajor;
	_updateMinor = a._updateMinor;
	return (*this);
}

AttributeBlockMemory &AttributeBlockMemory::addElement(std::shared_ptr<Data> const &data)
{
	{
		auto &element = std::find_if(_elements.begin(), _elements.end(), [&](AttributeElement const &attributeElement) -> bool {
			return (!attributeElement.element.lock() && attributeElement.range.z == data->getSize()); // search if an emplacement is alredy available.
		});
		if (element != _elements.end())
		{
			element->element = data;
			_updateMinor = true;
			return (*this);
		}
	}
	size_t offset = (!_elements.empty()) ? _elements.back().range.y : 0;
	_elements.emplace_back(AttributeElement(data, glm::vec3(offset, offset + data->getSize(), data->getSize()))); // for the range x = start, y = end, z = size
	_size += data->getSize();
	_updateMajor = true; // ask to reset memory pull
	return (*this);
}

AttributeBlockMemory &AttributeBlockMemory::updateMemory()
{
	if (_updateMajor)
	{
		_buffer->bind();
		_buffer->BufferData(_size);
		size_t offset;
		std::for_each(_elements.begin(), _elements.end(), [&](AttributeElement const &attributeElement){
			if (!attributeElement.element.lock()) // indicate no data to push
			{
				return;
			}
			auto &data = *attributeElement.element.lock();
			_buffer->BufferSubData(attributeElement.range.x, attributeElement.range.y, data.getData());
		});
		_buffer->unbind();
		_updateMajor = false;
	}
	if (_updateMinor)
	{
		_buffer->bind();
		std::for_each(_elements.begin(), _elements.end(), [&](AttributeElement const &attributeElement){
			auto &element = attributeElement.element.lock();
			if (element)
			{
				auto &data = *attributeElement.element.lock();
				_buffer->BufferSubData(attributeElement.range.x, attributeElement.range.y, data.getData());
			}
		});
		_buffer->unbind();
	}
	return (*this);
}

AttributeBlockMemory::AttributeElement::AttributeElement(std::weak_ptr<Data> const &element, glm::vec3 const &range) :
element(element),
range(range)
{

}