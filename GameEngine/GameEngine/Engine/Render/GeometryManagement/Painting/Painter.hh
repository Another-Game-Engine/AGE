#pragma once

# include <memory>
# include <vector>
# include <Render/Key.hh>
# include <Render/Program.hh>
# include <Render/GeometryManagement/Vertices.hh>
# include <Render/GeometryManagement/BufferPrograms.hh>

class Painter
{
public:
	Painter(std::vector<std::shared_ptr<Program>> const &programs, std::vector<GLenum> &&types);
	Painter(Painter &&move);

public:
	Key<Vertices> add_vertices(size_t nbrVertex, size_t nbrIndices);
	Painter &remove_vertices(Key<Vertices> &key);
	Vertices *get_vertices(Key<Vertices> const &key);
	Key<Program> get_key_program(std::string const &name);
	Key<Program> get_key_program(std::shared_ptr<Program> const &p);
	Program *get_program(Key<Program> const &program);
	Painter &update();
	Painter &draw(GLenum mode, Key<Program> const &program, std::vector<Key<Vertices>> const &drawList);
	Painter &draw(GLenum mode, std::shared_ptr<Program> const &p, std::vector<Key<Vertices>> const &drawList);

private:
	BufferPrograms _buffer;
	std::vector<std::shared_ptr<Program>> _programs;
	std::vector<Vertices> _vertices;
};