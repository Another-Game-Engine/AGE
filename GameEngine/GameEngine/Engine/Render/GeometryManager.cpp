#include <Render/GeometryManager.hh>
#include <Render/Data.hh>
#include <Render/Pool.hh>
#include <iostream>
#include <string>
#include <cassert>

#define DEBUG_MESSAGE(type, from, reason, return_type) \
	{ assert(0 && std::string(std::string(type) + ":from[" + std::string(from) + "] reason[" + std::string(reason) + "]").c_str()); return return_type; }

namespace gl
{
	GeometryManager::GeometryManager()
	{
	}

	GeometryManager::~GeometryManager()
	{

	}

	Key<VertexPool> GeometryManager::addVertexPool()
	{
		Key<VertexPool> key;
		_vertexPool[key] = VertexPool();
		return (key);
	}

	Key<IndexPool> GeometryManager::addIndexPool()
	{
		Key<IndexPool> key;
		_indexPool[key];
		return (key);
	}

	Key<VertexPool> GeometryManager::addVertexPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		Key<VertexPool> key;

		_vertexPool[key] = VertexPool(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (key);
	}

	Key<VertexPool> GeometryManager::getVertexPool(size_t target) const
	{
		if (target >= _vertexPool.size())
			return (Key<VertexPool>(KEY_DESTROY));
		auto &element = _vertexPool.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<IndexPool> GeometryManager::getIndexPool(size_t target) const
	{
		if (target >= _vertexPool.size())
			return (Key<IndexPool>(KEY_DESTROY));
		auto &element = _indexPool.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	size_t GeometryManager::nbrVertexPool() const
	{
		return (_vertexPool.size());
	}

	size_t GeometryManager::nbrIndexPool() const
	{
		return (_indexPool.size());
	}

	// the key will be set to empty
	GeometryManager &GeometryManager::rmVertexPool(Key<VertexPool> &key)
	{
		if (getVertexPool(key, "rmVertexPool") == NULL)
			return (*this);
		_vertexPool.erase(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndexPool(Key<IndexPool> &key)
	{
		if (getIndexPool(key, "rmIndexPool") == NULL)
			return (*this);
		_indexPool.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Vertices> GeometryManager::getVertices(size_t target) const
	{
		if (target >= _vertices.size())
			return (Key<Vertices>(KEY_DESTROY));
		auto &element = _vertices.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	size_t GeometryManager::getNbrVertices() const
	{
		return (_vertices.size());
	}

	Key<Vertices> GeometryManager::addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
	{
		Key<Vertices> key;

		_vertices[key] = Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers);
		return (key);
	}

	Key<Indices> GeometryManager::addIndices(size_t nbrIndices, uint32_t *buffer)
	{
		Key<Indices> key;

		_indices[key] = Indices(nbrIndices, buffer);
		return (key);
	}

	GeometryManager &GeometryManager::rmVertices(Key<Vertices> &key)
	{
		if (getVertices(key, "rmVertices") == NULL)
			return (*this);
		dettachVerticesToVertexPool(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndices(Key<Indices> &key)
	{
		if (getIndices(key, "rmIndices") == NULL)
			return (*this);
		dettachIndicesToIndexPool(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::attachVerticesToVertexPool(Key<Vertices> const &keyVertices, Key<VertexPool> const &keyPool)
	{
		Attach<Vertices, VertexPool> newAttach;
		if ((newAttach.pool = getVertexPool(keyPool, "attachVerticesToVertexPool")) == NULL || (newAttach.data = getVertices(keyVertices, "attachVerticesToVertexPool")) == NULL)
			return (*this);
		dettachVerticesToVertexPool(keyVertices);
		newAttach.element = newAttach.pool->addVertices(*newAttach.data);
		_vertexAttach.insert(std::make_pair(keyVertices, newAttach));
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToVertexPool(Key<Vertices> const &key)
	{
		Attach<Vertices, VertexPool> *attach;
		if ((attach = getVertexAttach(key, "dettachVerticesToVertexPool")) == NULL)
			return (*this);
		attach->pool->rmVertices(attach->element);
		_vertexAttach.erase(key);
		return (*this);
	}

	GeometryManager &GeometryManager::attachIndicesToIndexPool(Key<Indices> const &keyIndices, Key<IndexPool> const &keyPool)
	{
		Attach<Indices, IndexPool> newAttach;
		if ((newAttach.pool = getIndexPool(keyPool, "attachIndicesToIndexPool")) == NULL)
			return (*this);
		if ((newAttach.data = getIndices(keyIndices, "attachIndicesToIndexPool")) == NULL)
			return (*this);
		dettachIndicesToIndexPool(keyIndices);
		newAttach.element = newAttach.pool->addIndices(*newAttach.data);
		_indexAttach.insert(std::make_pair(keyIndices, newAttach));
		return (*this);
	}

	GeometryManager &GeometryManager::dettachIndicesToIndexPool(Key<Indices> const &key)
	{
		Attach<Indices, IndexPool> *attach;
		if ((attach = getIndexAttach(key, "dettachIndicesToIndexPool")) == NULL)
			return (*this);
		attach->pool->rmIndices(attach->element);
		_indexAttach.erase(key);
		return (*this);
	}

	GeometryManager &GeometryManager::attachIndexPoolToVertexPool(Key<VertexPool> const &keyVertex, Key<IndexPool> const &keyIndex)
	{
		VertexPool *vertexPool;
		if ((vertexPool = getVertexPool(keyVertex, "attachIndexPoolToVertexPool")) == NULL)
			return (*this);
		IndexPool *indexPool;
		if ((indexPool = getIndexPool(keyIndex, "attachIndexPoolToVertexPool")) == NULL)
			return (*this);
		vertexPool->attachIndexPoolToVertexPool(*indexPool);
		indexPool->attachVertexPoolToIndexPool(*vertexPool);
		return (*this);
	}

	GeometryManager &GeometryManager::dettachIndexPoolToVertexPool(Key<VertexPool> const &keyVertex, Key<IndexPool> const &keyIndex)
	{
		VertexPool *vertexPool;
		if ((vertexPool = getVertexPool(keyVertex, "dettachIndexPoolToVertexPool")) == NULL)
			return (*this);
		IndexPool *indexPool;
		if ((indexPool = getIndexPool(keyIndex, "dettachIndexPoolToVertexPool")) == NULL)
			return (*this);
		vertexPool->dettachIndexPoolToVertexPool();
		indexPool->dettachVertexPoolToIndexPool();
		return (*this);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Indices> const &keyIndices, Key<Vertices> const &keyVertices)
	{
		Attach<Indices, IndexPool> *indexAttach;
		Attach<Vertices, VertexPool> *vertexAttach;

		if ((indexAttach = getIndexAttach(keyIndices, "draw")) == NULL)
			return (*this);
		if ((vertexAttach = getVertexAttach(keyVertices, "draw")) == NULL)
			return (*this);
		indexAttach->pool->syncronisation();
		vertexAttach->pool->syncronisation();
		vertexAttach->pool->draw(mode, indexAttach->element, vertexAttach->element);
		return (*this);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Vertices> const &keyVertices)
	{
		Attach<Vertices, VertexPool> *vertexAttach;

		if ((vertexAttach = getVertexAttach(keyVertices, "draw")) == NULL)
			return (*this);
		vertexAttach->pool->syncronisation();
		vertexAttach->pool->draw(mode, vertexAttach->element);
		return (*this);
	}

	VertexPool *GeometryManager::getVertexPool(Key<VertexPool> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "key is destroy", NULL);
		if (_vertexPool.size() == 0)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "no element into vertex pool", NULL);
		if (_optimizerVertexPoolSearch.first == key)
			return (_optimizerVertexPoolSearch.second);
		auto &vertexPool = _vertexPool.find(key);
		if (vertexPool == _vertexPool.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "element not found", NULL);
		_optimizerVertexPoolSearch.first = key;
		_optimizerVertexPoolSearch.second = &vertexPool->second;
		return (&vertexPool->second);
	}

	IndexPool *GeometryManager::getIndexPool(Key<IndexPool> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "key is destroy", NULL);
		if (_indexPool.size() == 0)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "no element into index pool", NULL);
		if (_optimizerIndexPoolSearch.first == key)
			return (_optimizerIndexPoolSearch.second);
		auto &indexPool = _indexPool.find(key);
		if (indexPool == _indexPool.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "element not found", NULL);
		_optimizerIndexPoolSearch.first = key;
		_optimizerIndexPoolSearch.second = &indexPool->second;
		return (&indexPool->second);
	}

	Indices *GeometryManager::getIndices(Key<Indices> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "key is destroy", NULL);
		if (_indices.size() == 0)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "no element into indices", NULL);
		if (_optimizerIndicesSearch.first == key)
			return (_optimizerIndicesSearch.second);
		auto &indices = _indices.find(key);
		if (indices == _indices.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "element not found", NULL);
		_optimizerIndicesSearch.first = key;
		_optimizerIndicesSearch.second = &indices->second;
		return (&indices->second);
	}

	Vertices *GeometryManager::getVertices(Key<Vertices> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "key is destroy", NULL);
		if (_vertices.size() == 0)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "no element into vertices", NULL);
		if (_optimizerVerticesSearch.first == key)
			return (_optimizerVerticesSearch.second);
		auto &vertices = _vertices.find(key);
		if (vertices == _vertices.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "element not found", NULL);
		_optimizerVerticesSearch.first = key;
		_optimizerVerticesSearch.second = &vertices->second;
		return (&vertices->second);
	}

	GeometryManager::Attach<Vertices, VertexPool> *GeometryManager::getVertexAttach(Key<Vertices> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "key is destroy", NULL);
		if (_optimizerVertexAttachSearch.first == key)
			return (_optimizerVertexAttachSearch.second);
		if (_vertexAttach.size() == 0)
			return (NULL);
		auto &verticesAttach = _vertexAttach.find(key);
		if (verticesAttach == _vertexAttach.end())
			return (NULL);
		_optimizerVertexAttachSearch.first = key;
		_optimizerVertexAttachSearch.second = &verticesAttach->second;
		return (&verticesAttach->second);
	}

	GeometryManager::Attach<Indices, IndexPool> *GeometryManager::getIndexAttach(Key<Indices> const &key, std::string const &in)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp - " + in, "key is destroy", NULL);
		if (_indexAttach.size() == 0)
			return (NULL);
		if (_optimizerIndexAttachSearch.first == key)
			return (_optimizerIndexAttachSearch.second);
		auto &indexAttach = _indexAttach.find(key);
		if (indexAttach == _indexAttach.end())
			return (NULL);
		_optimizerIndexAttachSearch.first = key;
		_optimizerIndexAttachSearch.second = &indexAttach->second;
		return (&indexAttach->second);
	}
}