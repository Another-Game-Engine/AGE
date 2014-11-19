#pragma once

# include <array>
# include <vector>
# include <utility>
# include <memory>
# include <Render/Attribute.hh>
# include <Render/AttributeBlockMemory.hh>

template <typename type_t> class Key;
class Vertices;

class GraphicalMemory
{
public:
	GraphicalMemory();

public:
	Key<Vertices> addVertices(Vertices const &vertices);
	Key<Vertices> addVertices(Vertices &&vertices);
	GraphicalMemory &deleteVertices(Key<GraphicalMemory> const &memory);
	GraphicalMemory &setVertices(Key<GraphicalMemory> const &memory, Vertices const &vertices);
	GraphicalMemory &setVertices(Key<GraphicalMemory> const &memory, Vertices &&vertices);

private:
	void addVerticesToBlocksMemory(Vertices const &vertices);
private:
	std::vector<Vertices> _elements;
	std::array<AttributeBlockMemory, Attribute::END> _blocksMemory;
};