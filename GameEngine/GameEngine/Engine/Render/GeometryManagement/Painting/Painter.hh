#pragma once

#include <string>
#include <Utils/OpenGL.hh>
#include <Utils/MemoryPool.hpp>
#include <memory>
#include <vector>
#include <Render/Key.hh>
#include <Render/Program.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Buffer/BufferPrograms.hh>
#include <Render/Properties/Properties.hh>
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
		Painter &draw(GLenum mode, std::shared_ptr<Program> const &p, std::vector<Properties> const &propertiesList, std::vector<Key<Vertices>> const &drawList);
		void uniqueDraw(GLenum mode, std::shared_ptr<Program> const &program, Properties const &properties, const Key<Vertices> &vertice);
	private:
		BufferPrograms _buffer;
		std::vector<Vertices> _vertices;
		AGE::SpinLock _mutex;
		std::vector<Key<Properties>> _propertiesToRemove;
	};
}
