#pragma once

#include <string>
#include <Utils/OpenGL.hh>
#include <Utils/MemoryPool.hpp>
#include <memory>
#include <vector>
#include <Utils/Key.hh>
#include <Render/Program.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Buffer/BufferPrograms.hh>
#include <Utils/SpinLock.hpp>

namespace AGE
{
	class Painter
	{
	public:
		Painter(std::vector<std::pair<GLenum, std::string>> const &types);
		Painter(Painter &&move);
		~Painter() = default;
		Painter(Painter const &) = default;

	public:
		bool coherent(std::shared_ptr<Program> const &prog) const;
		bool coherent(std::vector<std::pair<GLenum, std::string>> const &types) const;
		Key<Vertices> add_vertices(size_t nbrVertex, size_t nbrIndices);

		Painter &remove_vertices(Key<Vertices> &key);
		Vertices *get_vertices(Key<Vertices> const &key);
		Painter &draw(GLenum mode, std::shared_ptr<Program> const &p, std::vector<Key<Vertices>> const &drawList);
		void uniqueDraw(GLenum mode, std::shared_ptr<Program> const &program, const Key<Vertices> &vertice);
		void instanciedDraw(GLenum mode, std::shared_ptr<Program> const &program, const Key<Vertices> &vertice, std::size_t count);
		void uniqueDrawBegin(std::shared_ptr<Program> const &program = nullptr);
		void uniqueDrawEnd();
		void instanciedDrawBegin(std::shared_ptr<Program> const &program = nullptr);
		void instanciedDrawEnd();
	private:
		BufferPrograms _buffer;
		std::vector<Vertices> _vertices;
		AGE::SpinLock _mutex;
		bool _isInUniqueDraw;
		bool _isInstanciedDraw;
	};
}
