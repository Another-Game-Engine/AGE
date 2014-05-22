#ifndef VERTICESMANAGER_HH_
# define VERTICESMANAGER_HH_

#include <Utils/OpenGL.hh>
#include <OpenGL/Key.hh>
#include <OpenGL/VertexBuffer.hh>
#include <Utils/Dependency.hpp>
#include <map>

namespace gl
{
	class Vertices;
	class VerticesPool;

	//!\file VerticesManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class VerticesManager
	//!\brief Handle the geometry of the render
	class VerticesManager : public Dependency
	{
	public:
		// constructor
		VerticesManager();
		~VerticesManager();

		// handle pools
		Key<VerticesPool> addPool();
		Key<VerticesPool> addPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		Key<VerticesPool> getPool(size_t index) const;
		size_t nbrPool() const;
		VerticesManager &rmPool(Key<VerticesPool> const &key);

		//handle Vertices
		Key<Vertices> addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers);
		VerticesManager &rmVertices(Key<Vertices> const &key);
		Key<Vertices> getVertices(size_t index) const;
		size_t getNbrVertices() const;

		// attach vertices to pools
		VerticesManager &attachVerticesToPool(Key<Vertices> const &vertices, Key<VerticesPool> const &pool);
		VerticesManager &dettachVerticesToPool(Key<Vertices> const &vertices);

	private:
		// data represent pools
		std::map<Key<VerticesPool>, VerticesPool> _pools;
		std::map<Key<Vertices>, Vertices> _vertices;
	};
}

#endif