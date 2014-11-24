#pragma once

# include <vector>
# include <utility>
# include <memory>
# include <Render/Attribute.hh>
# include <Render/AttributeBlockMemory.hh>
# include <Render/Vertices.hh>

template <typename type_t> class Key;

typedef uint8_t GraphicQuery;

class GraphicalMemory
{
public:
	GraphicalMemory();

public:
	GraphicalMemory &load(std::vector<Attribute> const &attributes);
	Key<Vertices> handle(Vertices const &vertices);
	Key<Vertices> handle(Vertices &&vertices);
	GraphicalMemory &quit(Key<Vertices> const &memory);
	GraphicalMemory &query(Key<Vertices> const &memory, GraphicQuery query);

private:
	bool isMatch(Vertices const &vertices) const;

private:
	std::vector<Vertices> _elements;
	std::vector<AttributeBlockMemory> _blocksMemory;
};
