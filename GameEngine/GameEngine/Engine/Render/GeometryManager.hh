#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/Buffer.hh>
#include <Render/VertexArray.hh>
#include <Render/Pool.hh>
#include <Utils/Dependency.hpp>
#include <map>
#include <Utils/Containers/Vector.hpp>

namespace gl
{
	class Vertices;

	const uint8_t nbrSimpleForm = 1;
	
	enum SimpleForm
	{
		QUAD = 0
	};

	class GeometryManager
	{
	public:
		template <typename TYPE, typename POOL>
		struct Attach
		{
			TYPE const *data;
			POOL *pool;
			Key<Pool::Element<TYPE>> element;
		};

	public:
		// constructor
		GeometryManager();
		~GeometryManager();

		GeometryManager &createSimpleForm();
		Key<Vertices> getSimpleForm(SimpleForm form);

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

		//handle Vertices and Indices
		Key<Vertices> addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers);
		Key<Indices> addIndices(size_t nbrIndices, uint32_t *buffers);
		GeometryManager &rmVertices(Key<Vertices> &key);
		GeometryManager &rmIndices(Key<Indices> &key);
		Key<Vertices> getVertices(size_t index) const;
		Key<Indices> getIndices(size_t index) const;
		size_t getNbrVertices() const;
		size_t getNbrIndices() const;

		// attach vertices to pools
		GeometryManager &attachVerticesToVertexPool(Key<Vertices> const &vertices, Key<VertexPool> const &pool);
		GeometryManager &dettachVerticesToVertexPool(Key<Vertices> const &vertices);
		GeometryManager &attachIndicesToIndexPool(Key<Indices> const &vertices, Key<IndexPool> const &pool);
		GeometryManager &dettachIndicesToIndexPool(Key<Indices> const &vertices);
		GeometryManager &attachIndexPoolToVertexPool(Key<VertexPool> const &vertexpool, Key<IndexPool> const &indicespool);
		GeometryManager &dettachIndexPoolToVertexPool(Key<VertexPool> const &vertexpool, Key<IndexPool> const &indexpool);

		// draw
		GeometryManager &draw(GLenum mode, Key<Indices> const &keyindices, Key<Vertices> const &keyVertice);
		GeometryManager &draw(GLenum mode, Key<Vertices> const &keyvertices);
	private:
		// simple form
		gl::Key<Vertices> *_simpleForm;

		// data represent pools
		std::map<Key<IndexPool>, IndexPool> _indexPool;
		std::map<Key<VertexPool>, VertexPool> _vertexPool;
		std::map<Key<Indices>, Indices>	_indices;
		std::map<Key<Vertices>, Vertices> _vertices;
		std::map<Key<Vertices>, Attach<Vertices, VertexPool>> _vertexAttach;
		std::map<Key<Indices>, Attach<Indices, IndexPool>> _indexAttach;
	
		std::pair<Key<IndexPool>, IndexPool *> _optimizerIndexPoolSearch;
		std::pair<Key<VertexPool>, VertexPool *> _optimizerVertexPoolSearch;
		std::pair<Key<Vertices>, Vertices *> _optimizerVerticesSearch;
		std::pair<Key<Indices>, Indices *> _optimizerIndicesSearch;
		std::pair<Key<Vertices>, Attach<Vertices, VertexPool> *> _optimizerVertexAttachSearch;
		std::pair<Key<Indices>, Attach<Indices, IndexPool> *> _optimizerIndexAttachSearch;

		// tool use in intern
		VertexPool *getVertexPool(Key<VertexPool> const &key, std::string const &in);
		IndexPool *getIndexPool(Key<IndexPool> const &key, std::string const &in);
		Indices *getIndices(Key<Indices> const &key, std::string const &in);
		Vertices *getVertices(Key<Vertices> const &key, std::string const &in);
		Attach<Vertices, VertexPool> *getVertexAttach(Key<Vertices> const &key, std::string const &in);
		Attach<Indices, IndexPool> *getIndexAttach(Key<Indices> const &key, std::string const &in);
	};
}