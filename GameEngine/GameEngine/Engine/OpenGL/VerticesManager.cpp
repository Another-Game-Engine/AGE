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

	Key<VerticesPool> VerticesManager::addPool()
	{
		Key<VerticesPool> key;
		_pools[key] = VerticesPool();
		return (key);
	}

	Key<VerticesPool> VerticesManager::addPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		Key<VerticesPool> key;

		_pools[key] = VerticesPool(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (key);
	}

	Key<VerticesPool> VerticesManager::getPool(size_t target) const
	{
		if (target >= _pools.size())
		{
			Key<VerticesPool> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
		auto &element = _pools.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
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
		_pools.erase(key);
		return (*this);
	}

	Key<Vertices> VerticesManager::getVertices(size_t target) const
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

	size_t VerticesManager::getNbrVertices() const
	{
		return (_vertices.size());
	}

	Key<Vertices> VerticesManager::addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
	{
		Key<Vertices> key;

		_vertices[key] = Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers);
		return (key);
	}

	VerticesManager &VerticesManager::rmVertices(Key<Vertices> const &key)
	{
		if (!key)
			return (*this);
		_vertices.erase(key);
		return (*this);
	}

}