#pragma once

# include <Utils/OpenGL.hh>
# include <Utils/MemoryPool.hpp>
# include <memory>
# include <vector>
# include <Render/Key.hh>
# include <Render/Program.hh>
# include <Render/GeometryManagement/Data/Vertices.hh>
# include <Render/GeometryManagement/Buffer/BufferPrograms.hh>

namespace AGE
{
	class Painter
	{
	public:
		Painter(std::vector<GLenum> const &types);
		Painter(Painter &&move);
		~Painter() = default;
		Painter(Painter const &) = default;

	public:
		bool coherent(std::vector<GLenum> const &types) const;
		Key<Vertices> add_vertices(size_t nbrVertex, size_t nbrIndices);
		Painter &remove_vertices(Key<Vertices> &key);
		Vertices *get_vertices(Key<Vertices> const &key);
		Painter &draw(GLenum mode, std::shared_ptr<Program> const &p, std::vector<Key<Vertices>> const &drawList);

	private:
		BufferPrograms _buffer;
		std::vector<Vertices> _vertices;
	};
}
