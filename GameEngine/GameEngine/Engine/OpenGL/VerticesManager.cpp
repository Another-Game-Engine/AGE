#include <OpenGL/VerticesManager.hh>
#include <OpenGL/Vertices.hh>
#include <OpenGL/VerticesPool.hh>


#define WARNING_NOT_FOUND(thing) \
	std::cerr << "Warning: cannot find " << thing << "." << std::endl; \

namespace gl
{
	VerticesManager::VerticesManager()
	{

	}

	VerticesManager::~VerticesManager()
	{

	}

	Key<VerticesPool> const &VerticesManager::addPool()
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (!_pools[index].first)
			{
				_pools[index].first = Key<VerticesPool>();
				_pools[index].second = VerticesPool();
				return (_pools[index].first);
			}
		}
		_pools.push_back(std::make_pair(Key<VerticesPool>(), VerticesPool()));
		return (_pools[_pools.size() - 1].first);
	}

	Key<VerticesPool> const &VerticesManager::addPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (!_pools[index].first)
			{
				_pools[index].first = Key<VerticesPool>();
				_pools[index].second = VerticesPool();
				return (_pools[index].first);
			}
		}
		_pools.push_back(std::make_pair(Key<VerticesPool>(), VerticesPool()));
		_pools.back().second.setData(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (_pools[_pools.size() - 1].first);
	}

	Key<VerticesPool> VerticesManager::getPool(size_t index) const
	{
		if (index >= _pools.size())
		{
			Key<VerticesPool> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
		return (_pools[index].first);
	}

	size_t VerticesManager::nbrPool() const
	{
		return (_pools.size());
	}

	// the key will be set to empty
	VerticesManager &VerticesManager::rmPool(Key<VerticesPool> const &key)
	{
		if (!key)
			return (*this);
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (_pools[index].first == key)
			{
				_pools[index].first.destroy();
				return (*this);
			}
		}
		return (*this);
	}

	Key<Vertices> VerticesManager::getVertices(size_t index) const
	{
		if (index >= _vertices.size())
		{
			Key<Vertices> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
		return (_vertices[index].first);
	}

	size_t VerticesManager::getNbrVertices() const
	{
		return (_vertices.size());
	}

	Key<Vertices> const &VerticesManager::addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
	{
		for (size_t index = 0; index < _vertices.size(); ++index)
		{
			if (!_vertices[index].first)
			{
				_vertices[index].second = Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers);
				_vertices[index].first = Key<Vertices>();
				return (_vertices[index].first);
			}
		}
		_vertices.push_back(std::make_pair(Key<Vertices>(), Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers)));
		return (_vertices[_vertices.size() - 1].first);
	}

	VerticesManager &VerticesManager::rmVertices(Key<Vertices> const &key)
	{
		if (!key)
			return (*this);
		for (size_t index = 0; index < _vertices.size(); ++index)
		{
			if (_vertices[index].first == key)
			{
				_vertices[index].first.destroy();
				return (*this);
			}
		}
		return (*this);
	}

}