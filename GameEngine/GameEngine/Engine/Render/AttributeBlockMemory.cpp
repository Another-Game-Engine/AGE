#include <Render/AttributeBlockMemory.hh>
#include <Render/Key.hh>
#include <Render/Data.hh>
#include <utility>

AttributeBlockMemory::AttributeBlockMemory():
_type(Attribute::Position),
_buffer(nullptr),
_update(true)
{

}

AttributeBlockMemory::AttributeBlockMemory(Attribute type):
_type(type),
_buffer((_type == Attribute::Indices) ? std::unique_ptr<Buffer>(std::make_unique<IndexBuffer>()) : std::unique_ptr<Buffer>(std::make_unique<VertexBuffer>())),
_update(true)
{
}

AttributeBlockMemory::AttributeBlockMemory(AttributeBlockMemory &&move) :
_type(move._type),
_elements(std::move(move._elements)),
_buffer(std::move(move._buffer)),
_update(move._update)
{
}

AttributeBlockMemory &AttributeBlockMemory::operator=(AttributeBlockMemory &&a)
{
	_type = a._type;
	_elements = std::move(a._elements);
	_buffer = std::move(a._buffer);
	_update = a._update;
	return (*this);
}

AttributeBlockMemory &AttributeBlockMemory::handle(std::shared_ptr<Data> const &data)
{
	_update = true;
	for (auto &element : _elements) {
		auto &data = element.lock();
		if (data) {
			element = data;
			return (*this);
		}
	}
	_elements.emplace_back(data);
	return (*this);
}

AttributeBlockMemory &AttributeBlockMemory::update()
{
	if (!_update){
		return (*this);
	}
	size_t sizeBuffer = 0;
	for (auto &element : _elements) { // count the new size of buffer and set the position of the data into it
		auto &data = element.lock();
		if (data) {
			data->range(glm::vec2(sizeBuffer, sizeBuffer + data->size()));
			sizeBuffer += data->size();
		}
	}
	_buffer->bind();
	_buffer->BufferData(sizeBuffer);
	for (auto &element : _elements) { // set the data in the vertex buffer object
		auto &data = element.lock();
		if (data) {
			_buffer->BufferSubData(data->range().x, data->size(), data->data());
		}
	}
	_buffer->unbind();
	return (*this);
}

bool AttributeBlockMemory::operator!=(Data const &data) const
{
	return (data != _type);
}

bool AttributeBlockMemory::operator==(Data const &data) const
{
	return (data == _type);
}
