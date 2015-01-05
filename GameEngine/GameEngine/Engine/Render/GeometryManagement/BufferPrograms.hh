#pragma once

# include <Utils/OpenGL.hh>
# include <Render/GeometryManagement/Buffer.hh>
# include <Render/GeometryManagement/Vertices.hh>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/Buffer/VertexArray.hh>
# include <Render/Key.hh>
# include <vector>

class BufferPrograms
{
public:
	BufferPrograms(std::vector<GLenum> &&types);
	BufferPrograms(BufferPrograms &&move);

public:
	bool push_back(Vertices &vertices);
	BufferPrograms &clear();
	size_t size() const;
	BufferPrograms &bind();
	BufferPrograms &unbind();
	BufferPrograms &update();
	Vertices *get_vertices(Key<Vertices> const &key);

private:
	std::vector<GLenum> _types;
	std::vector<Buffer> _buffers;
	Buffer _indices_buffer;
	VertexArray _vertex_array;
};