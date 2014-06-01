#include <OpenGL/GeometryManager.hh>
#include <OpenGL/Vertices.hh>
#include <OpenGL/Pool.hh>
#include <iostream>
#include <string>

#define DEBUG_MESSAGE(type, from, key_word, reason) \
	std::cerr << std::string(type) + ":from[" + std::string(from) + "].key-word[" + std::string(key_word) + "].reason[" + std::string(reason) + "]" << std::endl;


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
		{
			Key<VertexPool> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
		auto &element = _vertexPool.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<IndexPool> GeometryManager::getIndexPool(size_t target) const
	{
		if (target >= _vertexPool.size())
		{
			Key<IndexPool> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
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
		{
			Key<Vertices> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
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
		{
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "rmVertices", "key not valid")
			return (*this);
		}
		_vertices.erase(key);
		key.destroy();
		return (*this);
	}

	void GeometryManager::attachVerticesToPool(Key<Vertices> const &keyvertices, Pool &wherefind)
	{
		Attach newAttach;
		auto &vertices = _vertices.find(keyvertices)->second;
		auto &pool = wherefind;
		for (size_t index = 0; index < _attach.size(); ++index)
		{
			if (_attach[index].pool == &pool &&_attach[index].vertices == &vertices)
				return;
		}
		newAttach.element = pool.addVertices(vertices);
		newAttach.pool = &pool;
		newAttach.vertices = &vertices;
		_attach.push_back(newAttach);
	}

	void GeometryManager::dettachVerticesToPool(Key<Vertices> const &keyvertices, Pool &wherefind)
	{
		auto &vertices = _vertices.find(keyvertices)->second;
		auto &pool = wherefind;
		for (size_t index = 0; index < _attach.size(); ++index)
		{
			if (_attach[index].pool == &pool &&_attach[index].vertices == &vertices)
			{
				_attach[index].pool->rmVertices(_attach[index].element);
				_attach.erase(_attach.begin() + index);
				return;
			}
		}
	}

	GeometryManager &GeometryManager::attachVerticesToVertexPool(Key<Vertices> const &keyvertices, Key<VertexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
		{
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "attachVerticesToVertexPool", "key not valid")
			return (*this);
		}
		attachVerticesToPool(keyvertices, _vertexPool.find(keypool)->second);
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToVertexPool(Key<Vertices> const &keyvertices, Key<VertexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
		{
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "dettachVerticesToVertexPool", "key not valid")
			return (*this);
		}
		dettachVerticesToPool(keyvertices, _vertexPool.find(keypool)->second);
		return (*this);
	}

	GeometryManager &GeometryManager::attachVerticesToIndexPool(Key<Vertices> const &keyvertices, Key<IndexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
		{
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "attachVerticesToIndexPool", "key not valid")
				return (*this);
		}
		auto &pool = _indexPool.find(keypool)->second;
		attachVerticesToPool(keyvertices, pool);
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToIndexPool(Key<Vertices> const &keyvertices, Key<IndexPool> const &keypool)
	{
		if (!keyvertices || !keypool)
		{
			DEBUG_MESSAGE("Warning:", "GeometryManager.cpp", "dettachVerticesToVertexPool", "key not valid")
				return (*this);
		}
		dettachVerticesToPool(keyvertices, _indexPool.find(keypool)->second);
		return (*this);
	}

	GeometryManager &GeometryManager::draw(Key<Vertices> const &keyindices, Key<Vertices> const &keyVertice)
	{
		return (*this);
	}

	GeometryManager &GeometryManager::draw(Key<Vertices> const &keyvertices)
	{
		return (*this);
	}
}