#include <Render/GraphicalMemory.hh>
#include <Render/Key.hh>
#include <Render/Vertices.hh>
#include <Render/Data.hh>

GraphicalMemory::GraphicalMemory()
{
	size_t index = 0;
	std::for_each(_blocksMemory.begin(), _blocksMemory.end(), [&index](AttributeBlockMemory &element){
		element = std::move(AttributeBlockMemory(Attribute(index++))); // build all attributes objects
	});
}

Key<Vertices> GraphicalMemory::addVertices(Vertices const &vertices)
{
	_elements.emplace_back(vertices); // keep a local value on ram of vertices
	auto &element = _elements.back();
	std::for_each(element.begin(), element.end(), [&](std::shared_ptr<Data> const &data){
		_blocksMemory[*data].addElement(data); // insert the data inside vertices into the attribute object which shares the same type.
	});
	return (Key<Vertices>::createKeyWithId(_elements.size() - 1));
}

Key<Vertices> GraphicalMemory::addVertices(Vertices &&vertices)
{
	_elements.emplace_back(std::move(vertices)); // keep a local value on ram of vertices
	auto &element = _elements.back();
	std::for_each(element.begin(), element.end(), [&](std::shared_ptr<Data> const &data){
		_blocksMemory[*data].addElement(data); // insert the data inside vertices into the attribute object which shares the same type.
	});
	return (Key<Vertices>::createKeyWithId(_elements.size() - 1));
}

GraphicalMemory & GraphicalMemory::deleteVertices(Key<GraphicalMemory> const &memory)
{
	_elements.erase(_elements.begin() + memory.getId());
	return (*this);
}

GraphicalMemory & GraphicalMemory::setVertices(Key<GraphicalMemory> const &memory, Vertices const &vertices)
{
	_elements[memory.getId()] = vertices;
	return (*this);
}

GraphicalMemory & GraphicalMemory::setVertices(Key<GraphicalMemory> const &memory, Vertices &&vertices)
{
	_elements[memory.getId()] = std::move(vertices);
	return (*this);
}