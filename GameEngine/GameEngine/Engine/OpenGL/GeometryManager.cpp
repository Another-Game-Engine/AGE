#include <OpenGL/GeometryManager.hh>
#include <OpenGL/Data.hh>
#include <OpenGL/Pool.hh>
#include <iostream>
#include <string>
#include <cassert>

#define DEBUG_MESSAGE(type, from, key_word, reason) \
	{ assert(0 && std::string(std::string(type) + ":from[" + std::string(from) + "].key-word[" + std::string(key_word) + "].reason[" + std::string(reason) + "]").c_str());}
#define RETURN_KEY_CORRUPT(type) \
	{Key<type> key_corrupt; key_corrupt.destroy(); return (key_corrupt); }

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
			RETURN_KEY_CORRUPT(VertexPool)
		auto &element = _vertexPool.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<IndexPool> GeometryManager::getIndexPool(size_t target) const
	{
		if (target >= _vertexPool.size())
			RETURN_KEY_CORRUPT(IndexPool)
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
		if (!key)
			return (*this);
		auto element = _vertexPool.find(key);
		if (element == _vertexPool.end())
			return (*this);
		_vertexPool.erase(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndexPool(Key<IndexPool> &key)
	{
		if (!key)
			return (*this);
		auto element = _indexPool.find(key);
		if (element == _indexPool.end())
			return (*this);
		_indexPool.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Vertices> GeometryManager::getVertices(size_t target) const
	{
		if (target >= _vertices.size())
			RETURN_KEY_CORRUPT(Vertices)
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
		if (!key)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "rmVertices", "key not valid")
		auto &attach = _vertexAttach.find(key);
		if (attach == _vertexAttach.end())
			return (*this);
		dettachVerticesToVertexPool(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndices(Key<Indices> &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "rmVertices", "key not valid")
		auto &attach = _indexAttach.find(key);
		if (attach == _indexAttach.end())
			return (*this);
		dettachIndicesToIndexPool(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::attachVerticesToVertexPool(Key<Vertices> const &keyvertices, Key<VertexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "attachVerticesToVertexPool", "key not valid")
		auto &attach = _vertexAttach.find(keyvertices);
		Attach<Vertices, VertexPool> newAttach;
		auto &vertices = _vertices.find(keyvertices)->second;
		auto &pool = _vertexPool.find(keypool)->second;
		newAttach.pool = &pool;
		newAttach.data = &vertices;
		dettachVerticesToVertexPool(keyvertices);
		newAttach.element = pool.addVertices(vertices);
		_vertexAttach.insert(std::make_pair(keyvertices, newAttach));
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToVertexPool(Key<Vertices> const &keyvertices)
	{
		if (!keyvertices)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "dettachVerticesToVertexPool", "key not valid")
		auto &attach = _vertexAttach.find(keyvertices);
		if (attach != _vertexAttach.end())
		{
			attach->second.pool->rmVertices(attach->second.element);
			_vertexAttach.erase(keyvertices);
		}
		return (*this);
	}

	GeometryManager &GeometryManager::attachIndicesToIndexPool(Key<Indices> const &keyindices, Key<IndexPool> const &keypool)
	{
		if (!keyindices || !keypool)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp", "attachVerticesToIndexPool", "key not valid")
			auto &attach = _indexAttach.find(keyindices);
		Attach<Indices, IndexPool> newAttach;
		auto &indices = _indices.find(keyindices)->second;
		auto &pool = _indexPool.find(keypool)->second;
		newAttach.pool = &pool;
		newAttach.data = &indices;
		dettachIndicesToIndexPool(keyindices);
		newAttach.element = pool.addIndices(indices);
		newAttach.element = pool.addIndices(indices);
		_indexAttach.insert(std::make_pair(keyindices, newAttach));
		return (*this);
	}

	GeometryManager &GeometryManager::dettachIndicesToIndexPool(Key<Indices> const &keyindices)
	{
		if (!keyindices)
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp", "dettachVerticesToVertexPool", "key not valid")
		auto &attach = _indexAttach.find(keyindices);
		if (attach != _indexAttach.end())
		{
			attach->second.pool->rmIndices(attach->second.element);
			_indexAttach.erase(keyindices);
		}
		return (*this);
	}

	GeometryManager &GeometryManager::attachIndexPoolToVertexPool(Key<VertexPool> const &vertexpool, Key<IndexPool> const &indexpool)
	{
		auto &vp = _vertexPool.find(vertexpool);
		if (vp == _vertexPool.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp", "attachIndexPoolToVertexPool", "vertex pool doesn't exist")
		auto &ip = _indexPool.find(indexpool);
		if (ip == _indexPool.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp", "attachIndexPoolToVertexPool", "indices pool doesn't exist")
		vp->second.attachIndexPoolToVertexPool(ip->second);
		ip->second.attachVertexPoolToIndexPool(vp->second);
		return (*this);
	}

	GeometryManager &GeometryManager::dettachIndexPoolToVertexPool(Key<VertexPool> const &vertexpool, Key<IndexPool> const &indexpool)
	{
		auto &vp = _vertexPool.find(vertexpool);
		if (vp == _vertexPool.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp", "dettachIndexPoolToVertexPool", "vertex pool doesn't exist")
		auto &ip = _indexPool.find(indexpool);
		if (ip == _indexPool.end())
			DEBUG_MESSAGE("Warning", "GeometryManager.cpp", "attachIndexPoolToVertexPool", "indices pool doesn't exist")
		vp->second.dettachIndexPoolToVertexPool();
		ip->second.dettachVertexPoolToIndexPool();
		return (*this);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Indices> const &keyindices, Key<Vertices> const &keyVertice)
	{
		if (!keyindices || !keyVertice)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "key not valid")
		auto &indiceAttach = _indexAttach.find(keyindices);
		auto &vertexAttach = _vertexAttach.find(keyVertice);
		if (indiceAttach == _indexAttach.end() || vertexAttach == _vertexAttach.end())
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "keyindices of keyvertices have no attach")
		VertexPool *vertexPool = static_cast<VertexPool *>(vertexAttach->second.pool);
		indiceAttach->second.pool->syncronisation();
		vertexAttach->second.pool->syncronisation();
		vertexPool->draw(mode, indiceAttach->second.element, vertexAttach->second.element);
		return (*this);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Vertices> const &keyvertices)
	{
		if (!keyvertices)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "key not valid")
		auto &vertexAttach = _vertexAttach.find(keyvertices);
		if (vertexAttach == _vertexAttach.end())
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "keyindices have no attach")
		VertexPool *vertexPool = vertexAttach->second.pool;
		vertexAttach->second.pool->syncronisation();
		vertexPool->draw(mode, vertexAttach->second.element);
		return (*this);
	}
}