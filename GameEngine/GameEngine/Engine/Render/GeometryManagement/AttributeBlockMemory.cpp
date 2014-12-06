#include <Render/GeometryManagement/AttributeBlockMemory.hh>
#include <Render/Key.hh>
#include <Render/GeometryManagement/Data.hh>
#include <utility>
#include <Render/Buffer/VertexBuffer.hh>
#include <Render/Buffer/IndexBuffer.hh>

AttributeBlockMemory::AttributeBlockMemory():
_type(Attribute::Position),
_buffer(nullptr),
_update(true)
{

}

AttributeBlockMemory::AttributeBlockMemory(Attribute type):
_type(type),
_buffer((_type == Attribute::Indices) ? std::unique_ptr<IBuffer>(std::make_unique<IndexBuffer>()) : std::unique_ptr<IBuffer>(std::make_unique<VertexBuffer>())),
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

/**
* Method:    handle
* FullName:  AttributeBlockMemory::handle
* Access:    public 
* Returns:   AttributeBlockMemory &
* Qualifier:
* Parameter: std::shared_ptr<Data> const & data
* Goal:		handle the Data in argument but do not handle the ownership of the ptr
*/
AttributeBlockMemory &AttributeBlockMemory::add(std::shared_ptr<Data> const &data)
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

/**
* Method:    update
* FullName:  AttributeBlockMemory::update
* Access:    public 
* Returns:   AttributeBlockMemory &
* Qualifier:
* Goal:		 update the memory on GPU if there are modifications
*/
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
	_buffer->alloc(sizeBuffer);
	for (auto &element : _elements) { // set the data in the vertex buffer object
		auto &data = element.lock();
		if (data) {
			_buffer->sub(data->range().x, data->size(), data->data());
		}
	}
	_buffer->unbind();
	return (*this);
}

/**
* Method:    buffer
* FullName:  AttributeBlockMemory::buffer
* Access:    public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 get the buffer
*/
IBuffer const & AttributeBlockMemory::buffer() const
{
	return (*_buffer.get());
}

/**
* Method:    operator!=
* FullName:  AttributeBlockMemory::operator!=
* Access:    public 
* Returns:   bool
* Qualifier: const
* Parameter: Attribute attribute
* Goal:		 compare the argument and the attribute type of the instance
*/
bool AttributeBlockMemory::operator!=(Attribute attribute) const
{
	return (attribute != _type);
}

/**
* Method:    operator==
* FullName:  AttributeBlockMemory::operator==
* Access:    public 
* Returns:   bool
* Qualifier: const
* Parameter: Attribute attribute
* Goal:		 compare the argument and the attribute type of the instance
*/
bool AttributeBlockMemory::operator==(Attribute attribute) const
{
	return (attribute == _type);
}

/**
* Method:    operator size_t
* FullName:  AttributeBlockMemory::operator size_t
* Access:    public 
* Returns:   
* Qualifier: const
* Goal:		 cast the AttributeBlockMemory in size_t with the attribute type of the instance
*/
AttributeBlockMemory::operator Attribute() const
{
	return (_type);
}
