#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/Buffer.hh>
#include <Render/VertexArray.hh>
#include <Render/Pool.hh>
#include <Utils/Dependency.hpp>
#include <map>
#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>

namespace gl
{
	class Vertices;
	class Indices;
	template <typename TYPE> struct Element;
	class VertexPool;
	class IndexPool;

	enum SimpleForm
	{
		QUAD = 0,
		SPHERE,
		NBR_SIMPLE_FORM
	};

	template <typename TYPE, typename POOL>
	struct Attach
	{
		POOL *pool;
		Key<Element<TYPE>> element;
		Attach(POOL *p) : pool(p), element(Key<Element<TYPE>>::createKey()) {}
		Attach() : pool(NULL){}
		Attach(Attach<TYPE, POOL> const &copy) : pool(copy.pool), element(copy.element){}
		Attach &operator=(Attach<TYPE, POOL> const &a) { pool = a.pool; element = a.element; return (*this); }
	};

	class GeometryManager
	{
	public:
		// constructor
		GeometryManager();
		~GeometryManager();

		GeometryManager &createQuadSimpleForm();
		GeometryManager &createSphereSimpleForm();
		Key<Vertices> getSimpleFormGeo(SimpleForm form);
		Key<Indices> getSimpleFormId(SimpleForm form);

		// handle pools
		Key<VertexPool> addVertexPool();
		Key<VertexPool> addVertexPool(uint8_t nbrAttributes, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t>const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent);
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
		
		// generate ico sphere
		static void generateIcoSphere(size_t recursion, glm::vec3 **vertex, glm::u32vec3 **indices, size_t &nbrElementId, size_t &nbrElementGeo);

		GeometryManager &draw(GLenum mode, Key<Vertices> const &vertices);
		GeometryManager &draw(GLenum mode, Key<Indices> const &indices, Key<Vertices> const &vertices);

	private:
		// simple form
		std::map<SimpleForm, Key<Vertices>> _simpleFormGeo;
		std::map<SimpleForm, Key<Indices>> _simpleFormId;
		Key<VertexPool> *_simpleFormPoolGeo;
		Key<IndexPool> *_simpleFormPoolId;
		void initSimpleForm();

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
		std::pair<VertexPool *, VertexArray *> _optimizerDrawingContextSearch;

		// tool use in intern
		VertexPool *getVertexPool(Key<VertexPool> const &key);
		IndexPool *getIndexPool(Key<IndexPool> const &key);
		Indices *getIndices(Key<Indices> const &key);
		Vertices *getVertices(Key<Vertices> const &key);
		Attach<Vertices, VertexPool> *getVertexAttach(Key<Vertices> const &key);
		Attach<Indices, IndexPool> *getIndexAttach(Key<Indices> const &key);
	};

}