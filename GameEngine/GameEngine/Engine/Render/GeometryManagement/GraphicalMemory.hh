#pragma once

# include <vector>
# include <utility>
# include <memory>
# include <Render/GeometryManagement/Attribute.hh>
# include <Render/GeometryManagement/AttributeBlockMemory.hh>
# include <Render/GeometryManagement/Vertices.hh>

template <typename type_t> class Key;

class GraphicalMemory
{
public:
	GraphicalMemory();

public:
	GraphicalMemory &load(std::vector<Attribute> const &attributes);
	Key<Vertices> handle(Vertices const &vertices);
	Key<Vertices> handle(Vertices &&vertices);
	GraphicalMemory &unhandle(Key<Vertices> &memory);
	GraphicalMemory &draw(GLenum mode, Key<Vertices> &memory);
	GraphicalMemory &bind();
	GraphicalMemory &update();

private:
	bool _is_match(Vertices const &vertices) const;
	bool _is_has_indices() const;

private:
	std::vector<Vertices> _elements;
	std::vector<AttributeBlockMemory> _blocksMemory;

};