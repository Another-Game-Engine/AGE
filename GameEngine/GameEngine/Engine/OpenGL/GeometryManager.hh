#ifndef GEOMETRYMANAGER_HH_
# define GEOMETRYMANAGER_HH_

#include <Utils/OpenGL.hh>
#include <OpenGL/Key.hh>
#include <OpenGL/Buffer.hh>
#include <OpenGL/VertexArray.hh>
#include <OpenGL/Pool.hh>
#include <Utils/Dependency.hpp>
#include <map>
#include <vector>

namespace gl
{
	class Vertices;

	//!\file VerticesManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class VerticesManager
	//!\brief Handle the geometry of the render
	class GeometryManager : public Dependency
	{
	public:
		struct Attach
		{
			Vertices const *vertices;
			Pool *pool;
			Key<Pool::PoolElement> element;
		};
	public:
		// constructor
		GeometryManager();
		~GeometryManager();

		// handle pools
		Key<VertexPool> addVertexPool();
		Key<VertexPool> addVertexPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		Key<VertexPool> getVertexPool(size_t index) const;
		size_t nbrVertexPool() const;
		GeometryManager &rmVertexPool(Key<VertexPool> &key);
		Key<IndexPool> addIndexPool();
		Key<IndexPool> getIndexPool(size_t index) const;
		size_t nbrIndexPool() const;
		GeometryManager &rmIndexPool(Key<IndexPool> &key);

		//handle Vertices
		Key<Vertices> addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers);
		GeometryManager &rmVertices(Key<Vertices> &key);
		Key<Vertices> getVertices(size_t index) const;
		size_t getNbrVertices() const;

		// attach vertices to pools
		GeometryManager &attachVerticesToVertexPool(Key<Vertices> const &vertices, Key<VertexPool> const &pool);
		GeometryManager &dettachVerticesToVertexPool(Key<Vertices> const &vertices, Key<VertexPool> const &pool);
		GeometryManager &attachVerticesToIndexPool(Key<Vertices> const &vertices, Key<IndexPool> const &pool);
		GeometryManager &dettachVerticesToIndexPool(Key<Vertices> const &vertices, Key<IndexPool> const &pool);
	private:
		void attachVerticesToPool(Key<Vertices> const &vertices, Pool &wherefind);
		void dettachVerticesToPool(Key<Vertices> const &vertices, Pool &wherefind);
	public:

		// draw
		GeometryManager &draw(Key<Vertices> const &keyindices, Key<Vertices> const &keyVertice);
		GeometryManager &draw(Key<Vertices> const &keyvertices);
	private:
		// data represent pools
		std::map<Key<IndexPool>, IndexPool> _indexPool;
		std::map<Key<VertexPool>, VertexPool> _vertexPool;
		std::map<Key<Vertices>, Vertices> _vertices;
		std::vector<Attach> _attach;
	};
}

#endif