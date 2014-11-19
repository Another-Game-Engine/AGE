#include <Render/GraphicalMemory.hh>
#include <Render/Key.hh>
#include <Render/Vertices.hh>

GraphicalMemory::GraphicalMemory()
{
	size_t index = 0;
	std::for_each(_blocksMemory.end(), _blocksMemory.begin(), [&](AttributeBlockMemory &element){
		element = std::move(AttributeBlockMemory(Attribute(index++)));
	});
}

Key<Vertices> GraphicalMemory::addVertices(Vertices const &vertices)
{
	_elements.emplace_back(vertices);
	auto &element = _elements.back();
	addVerticesToBlocksMemory(vertices);
	return (Key<Vertices>::createKeyWithId(_elements.size() - 1));
}

Key<Vertices> GraphicalMemory::addVertices(Vertices &&vertices)
{
	_elements.emplace_back(std::move(vertices));
	auto &element = _elements.back();
	addVerticesToBlocksMemory(std::move(vertices));
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

void GraphicalMemory::addVerticesToBlocksMemory(Vertices const &vertices)
{
	std::for_each()
	for (size_t index = 0; index < vertices.nbrData(); ++index)
	{
		auto &tmp = vertices.getData(index);
	}
}