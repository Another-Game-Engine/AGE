#include <OpenGL/GeometryManager.hh>
#include <OpenGL/Vertices.hh>
#include <OpenGL/Pool.hh>
#include <iostream>
#include <string>

#define DEBUG_MESSAGE(type, from, key_word, reason) \
	{ std::cerr << std::string(type) + ":from[" + std::string(from) + "].key-word[" + std::string(key_word) + "].reason[" + std::string(reason) + "]" << std::endl; return (*this); }
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
		_indexPool[key] = IndexPool();
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
		_vertexPool.erase(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndexPool(Key<IndexPool> &key)
	{
		if (!key)
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

	GeometryManager &GeometryManager::rmVertices(Key<Vertices> &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "rmVertices", "key not valid")
		_vertices.erase(key);
		key.destroy();
		return (*this);
	}

	void GeometryManager::attachVerticesToPool(Key<Vertices> const &keyvertices, Pool &wherefind)
	{
		auto &attach = _attach.find(keyvertices);
		Attach newAttach;
		auto &vertices = _vertices.find(keyvertices)->second;
		auto &pool = wherefind;
		newAttach.element = pool.addVertices(vertices);
		newAttach.pool = &pool;
		newAttach.vertices = &vertices;
		if (attach != _attach.end())
		{
			attach->second.pool->rmVertices(attach->second.element);
			attach->second = newAttach;
		}
		else
			_attach.insert(std::make_pair(keyvertices, newAttach));
	}

	void GeometryManager::dettachVerticesToPool(Key<Vertices> const &keyvertices, Pool &wherefind)
	{
		auto &vertices = _vertices.find(keyvertices)->second;
		auto &pool = wherefind;
		auto &attach = _attach.find(keyvertices);
		if (attach != _attach.end())
		{
			attach->second.pool->rmVertices(attach->second.element);
			_attach.erase(keyvertices);
		}
	}

	GeometryManager &GeometryManager::attachVerticesToVertexPool(Key<Vertices> const &keyvertices, Key<VertexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "attachVerticesToVertexPool", "key not valid")
		attachVerticesToPool(keyvertices, _vertexPool.find(keypool)->second);
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToVertexPool(Key<Vertices> const &keyvertices, Key<VertexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "dettachVerticesToVertexPool", "key not valid")
		dettachVerticesToPool(keyvertices, _vertexPool.find(keypool)->second);
		return (*this);
	}

	GeometryManager &GeometryManager::attachVerticesToIndexPool(Key<Vertices> const &keyvertices, Key<IndexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "attachVerticesToIndexPool", "key not valid")
		auto &pool = _indexPool.find(keypool)->second;
		attachVerticesToPool(keyvertices, pool);
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToIndexPool(Key<Vertices> const &keyvertices, Key<IndexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "dettachVerticesToVertexPool", "key not valid")
		dettachVerticesToPool(keyvertices, _indexPool.find(keypool)->second);
		return (*this);
	}

	GeometryManager &GeometryManager::draw(Key<Vertices> const &keyindices, Key<Vertices> const &keyVertice)
	{
		if (!keyindices || !keyVertice)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "key not valid")
		auto &indiceAttach = _attach.find(keyindices);
		auto &vertexAttach = _attach.find(keyVertice);
		if (indiceAttach == _attach.end() || vertexAttach == _attach.end())
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "keyindices have no attach")
		VertexPool *vertexPool = static_cast<VertexPool *>(vertexAttach->second.pool);
		indiceAttach->second.pool->syncronisation();
		vertexAttach->second.pool->syncronisation();
		vertexPool->draw(indiceAttach->second.element, vertexAttach->second.element);
		return (*this);
	}

	GeometryManager &GeometryManager::draw(Key<Vertices> const &keyvertices)
	{
		if (!keyvertices)
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "key not valid")
		auto &vertexAttach = _attach.find(keyvertices);
		if (vertexAttach == _attach.end())
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "draw", "keyindices have no attach")
		VertexPool *vertexPool = static_cast<VertexPool *>(vertexAttach->second.pool);
		vertexAttach->second.pool->syncronisation();
		vertexPool->draw(vertexAttach->second.element);
		return (*this);
	}
}