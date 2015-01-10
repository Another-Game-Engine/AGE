#pragma once

# include <Utils/OpenGL.hh>
# include <Render/GeometryManagement/Buffer.hh>
# include <Render/GeometryManagement/Vertices.hh>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/Buffer/VertexArray.hh>
# include <Render/Key.hh>
# include <vector>

class Program;

class BufferPrograms
{
public:
	BufferPrograms(std::vector<GLenum> const &types);
	BufferPrograms(BufferPrograms &&move);

public:
	bool insert(Vertices &vertices);
	BufferPrograms &clear();
	size_t size() const;
	BufferPrograms &bind();
	BufferPrograms &unbind();
	BufferPrograms &update();
	bool coherent_program(std::shared_ptr<Program> const &p);
	std::vector<GLenum> const &get_types() const;

private:
	std::vector<GLenum> _types;
	std::vector<Buffer> _buffers;
	Buffer _indices_buffer;
	VertexArray _vertex_array;
};