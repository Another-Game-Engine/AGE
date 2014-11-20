#include <Render/GraphicalMemory.hh>
#include <Render/Key.hh>
#include <Render/Vertices.hh>
#include <Render/Data.hh>

GraphicalMemory::GraphicalMemory()
{
}

GraphicalMemory &GraphicalMemory::load(std::vector<Attribute> const &attributes)
{
	return (*this);
}

GraphicalMemory &GraphicalMemory::load(std::vector<Attribute> &&attribtues)
{
	return (*this);
}

Key<Vertices> GraphicalMemory::handle(Vertices const &vertices)
{
}

Key<Vertices> GraphicalMemory::handle(Vertices &&vertices)
{
}

GraphicalMemory & GraphicalMemory::quit(Key<Vertices> const &memory)
{
	return (*this);
}