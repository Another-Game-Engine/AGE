#include <OpenGL/VertexManager.hh>

#if TEST_NEW_VERTEXMANAGER

namespace gl
{
	VerticesManager::VerticesManager()
	{

	}

	VerticesManager::~VerticesManager()
	{

	}

	Key<VerticesPool> const &VerticesManager::addPool(VerticesPool const &pool)
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (_pools[index].first == false)
			{
				_pools[index].first = true;
				_pools[index].second = pool;
				return (Key<VerticesPool>(index));
			}
		}
		_pools.push_back(std::make_pair(true, pool));
		return (Key<VerticesPool>(_pools.size() - 1));
	}

	Key<VerticesPool> const &VerticesManager::getPool(size_t index) const
	{
		if (index >= _pools.size())
			return (Key<VerticesPool>(-1));
		return (Key<VerticesPool>(index));
	}

	size_t VerticesManager::nbrPool() const
	{
		return (_pools.size());
	}

	void VerticesManager::rmPool(Key<VerticesPool> const &key)
	{
		if ((key.getId() < _pools.size()) && key.empty() == false)
			_pools[key.getId()].first = false;
	}

	void VerticesManager::clearPool()
	{
		_pools.clear();
	}

	Key<Vertices> const &VerticesManager::addVertices(Vertices const &vertices)
	{
		for (size_t index = 0; index < _vertices.size(); ++index)
		{
			if (_vertices[index].first == false)
			{
				_vertices[index].first = true;
				_vertices[index].second = vertices;
				return (Key<Vertices>(index));
			}
		}
		_vertices.push_back(std::make_pair(true, vertices));
		return (Key<Vertices>(_vertices.size() - 1));
	}

	void VerticesManager::rmVertices(Key<Vertices> const &key)
	{
		if ((key.getId() < _vertices.size()) && key.empty() == false)
			_vertices[key.getId()].first = false;
	}

	Key<Vertices> const &VerticesManager::getVertices(size_t index) const
	{
		if (index >= _vertices.size())
			return (Key<Vertices>(-1));
		return (Key<Vertices>(index));
	}

	size_t VerticesManager::nbrVertices() const
	{
		return (_vertices.size());
	}

	void VerticesManager::clearVertices()
	{
		_vertices.clear();
	}
}

#endif /*!TEST_NEW_VERTEXMANAGER*/