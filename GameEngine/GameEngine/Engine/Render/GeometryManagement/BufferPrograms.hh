#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Program.hh>
# include <Render/GeometryManagement/Buffer.hh>
# include <Render/GeometryManagement/Vertices.hh>
# include <Render/ProgramResources/AProgramResources.hh>
# include <Render/Buffer/VertexArray.hh>
# include <Render/Key.hh>
# include <vector>

class BufferPrograms
{
public:
	BufferPrograms(std::vector<GLenum> &&types, std::vector<std::shared_ptr<Program>> const &programs);
	BufferPrograms(BufferPrograms &&move);

public:
	Key<Vertices> push_back(Vertices &vertices);
	BufferPrograms &pop_back();
	size_t size() const;
	BufferPrograms &bind();
	BufferPrograms &unbind();
	BufferPrograms &update();
	Buffer &operator[](Key<Vertices> const &key);

private:
	std::vector<GLenum> _types;
	std::vector<Buffer> _buffers;
	std::vector<std::shared_ptr<Program>> _programs;
	std::vector<Vertices> _vertices;
	VertexArray _vertex_array;
};