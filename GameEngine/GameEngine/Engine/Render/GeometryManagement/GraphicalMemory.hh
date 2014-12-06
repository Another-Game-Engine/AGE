#pragma once

# include <vector>
# include <utility>
# include <memory>
# include <Render/GeometryManagement/Attribute.hh>
# include <Render/GeometryManagement/AttributeBlockMemory.hh>
# include <Render/GeometryManagement/Vertices.hh>
# include <Render/Buffer/VertexArray.hh>

template <typename type_t> class Key;

class GraphicalMemory
{
public:
	GraphicalMemory();

public:
	GraphicalMemory &init(std::vector<Attribute> const &attributes);
	Key<Vertices> add(Vertices const &vertices);
	Key<Vertices> add(Vertices &&vertices);
	Vertices &get(Key<Vertices> const &memory);
	GraphicalMemory &remove(Key<Vertices> &memory);
	GraphicalMemory &draw(GLenum mode, Key<Vertices> &memory);
	GraphicalMemory &bind();
	GraphicalMemory &update();

private:
	bool _is_match(Vertices const &vertices) const;
	bool _is_has_indices() const;

private:
	VertexArray _array_buffer;
	std::vector<Vertices> _elements;
	std::vector<AttributeBlockMemory> _blocksMemory;

};