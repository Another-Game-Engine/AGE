#pragma once

# include <Utils/OpenGL.hh>
# include <Utils/MemoryPool.hpp>
# include <memory>
# include <vector>
# include <Render/Key.hh>
# include <Render/Program.hh>
# include <Render/GeometryManagement/Data/Vertices.hh>
# include <Render/GeometryManagement/Buffer/BufferPrograms.hh>
# include <Render/Properties/Properties.hh>
#include <Utils/SpinLock.hpp>

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

		// will alloc property (used by the render thread only)
		Key<Properties> add_properties(std::shared_ptr<Properties> const &properties);
		// create property key, do not alloc, can be called by other threads
		Key<Properties> reserve_properties();
		// alloc reserved property, should be called by render thread only
		void alloc_reserved_properties(const Key<Properties> &key, std::shared_ptr<Properties> const &properties);


		Painter &remove_vertices(Key<Vertices> &key);
		Painter &remove_properties(Key<Properties> &key);
		Vertices *get_vertices(Key<Vertices> const &key);
		std::shared_ptr<Properties> get_properties(Key<Properties> const &key) const;
		Painter &draw(GLenum mode, std::shared_ptr<Program> const &p, std::vector<Key<Properties>> const &propertiesList, std::vector<Key<Vertices>> const &drawList);

	private:
		BufferPrograms _buffer;
		std::vector<Vertices> _vertices;
		MemoryPool<std::shared_ptr<Properties>> _properties;
		AGE::SpinLock _mutex;
		std::vector<Key<Properties>> _propertiesToRemove;
	};
}
