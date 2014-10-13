#include <Render/Pool.hh>

#define INDEXPOOL 1

namespace gl
{
	AttributeData::AttributeData()
		: typeComponent(GL_FLOAT),
		sizeTypeComponent(sizeof(float)),
		nbrComponent(4),
		sizeAttribute(0),
		offsetAttribute(0)
	{

	}

	AttributeData::AttributeData(GLenum tc, uint8_t stc, uint8_t nbrC)
		: typeComponent(tc),
		sizeTypeComponent(stc),
		nbrComponent(nbrC),
		sizeAttribute(0),
		offsetAttribute(0)
	{

	}

	AttributeData::AttributeData(AttributeData const &copy)
		: typeComponent(copy.typeComponent),
		sizeTypeComponent(copy.sizeTypeComponent),
		nbrComponent(copy.nbrComponent),
		sizeAttribute(copy.sizeAttribute),
		offsetAttribute(copy.offsetAttribute)
	{

	}

	AttributeData &AttributeData::operator=(AttributeData const &a)
	{
		typeComponent = a.typeComponent;
		sizeTypeComponent = a.sizeTypeComponent;
		nbrComponent = a.nbrComponent;
		sizeAttribute = a.sizeAttribute;
		offsetAttribute = a.offsetAttribute;
		return (*this);
	}

	VertexPool::VertexPool()
		: Pool(),
		_indexPoolattach(NULL)
	{
	}

	VertexPool::VertexPool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent)
		: Pool<VertexPool, Vertices>(nbrAttribute, typeComponent, sizeTypeComponent, nbrComponent),
		_indexPoolattach(NULL)
	{
	}

	VertexPool::~VertexPool()
	{
	}

	Pool<VertexPool, Vertices> &VertexPool::syncronisation()
	{
		if (!_syncronized || !_internalSyncronized)
			_vbo.bind();
		if (!_syncronized)
			_vbo.BufferData(_nbrBytePool);
		if (!_internalSyncronized)
		{
			if (_nbrAttribute)
				memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_nbrElementPool = 0;
			for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
			{
				Vertices const *vertices = index->second.data;
				MemoryBlocksGPU &memory = _poolMemory[index->second.memoryIndex];
				if (vertices && memory.getSync() == false)
				{
					memory.setStartElement(_nbrElementPool);
					for (uint8_t index = 0; index < _nbrAttribute; ++index)
					{
						memory.setSync(true);
						memory.setOffset(index, _offsetAttribute[index] + _sizeAttribute[index]);
						_vbo.BufferSubData(memory.getOffset(index), memory.getSizeBlock(index), (void *)vertices->getBuffer(index));
						_sizeAttribute[index] += memory.getSizeBlock(index);
					}
					_nbrElementPool += memory.getNbrElement();
				}
			}
		}
		if (!_syncronized)
		{
			_vao.bind();
			_vbo.bind();
			if (_indexPoolattach)
				_indexPoolattach->getBuffer().bind();
			for (size_t index = 0; index < _nbrAttribute; ++index)
			{
				_vao.activateAttribute(index);
				_vao.attribute(index, _nbrComponent[index], _typeComponent[index], _offsetAttribute[index]);
			}
			_vao.unbind();
		}
		_internalSyncronized = true;
		_syncronized = true;
		return (*this);
	}

	VertexPool const &VertexPool::draw(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt) const
	{
		if (!_indexPoolattach) assert(0);
		_vao.bind();
		auto &element = _poolElement.find(drawOnIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		_indexPoolattach->onDrawCall(mode, drawWithIt, memory);
		_vao.unbind();
		return (*this);
	}

	VertexPool const &VertexPool::drawInstanced(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt, size_t nbrIntanced) const
	{
		if (!_indexPoolattach) assert(0);
		_vao.bind();
		auto &element = _poolElement.find(drawOnIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		_indexPoolattach->onDrawIntancedCall(mode, drawWithIt, memory, nbrIntanced);
		_vao.unbind();
		return (*this);
	}


	VertexPool &VertexPool::operator=(VertexPool const &v)
	{
		if (this != &v)
		{
			clearPool();
			Pool::operator=(v);
			_indexPoolattach = v._indexPoolattach;
			return (*this);
		}
		return (*this);
	}

	VertexPool const &VertexPool::draw(GLenum mode, Key<Element<Vertices>> const &drawIt) const
	{
		Pool::Element<Vertices> const *element;
		if ((element = getVerticesPoolElement(drawIt, "draw")) == NULL)
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->memoryIndex];
		_vao.bind();
		glDrawArrays(mode, GLint(memory.getElementStart()), GLsizei(memory.getNbrElement()));
		return (*this);
	}

	VertexPool &VertexPool::attachIndexPoolToVertexPool(IndexPool const &pool)
	{
		_indexPoolattach = &pool;
		return (*this);
	}

	VertexPool &VertexPool::dettachIndexPoolToVertexPool()
	{
		_indexPoolattach = NULL;
		return (*this);
	}

	Buffer const & VertexPool::getBuffer() const
	{
		return (_vbo);
	}

	IndexPool::IndexPool()
		: Pool(INDEXPOOL),
		_vertexPoolattach(NULL)
	{
	}

	IndexPool::~IndexPool()
	{
	}

	IndexPool::IndexPool(IndexPool const &copy)
		: Pool(copy),
		_vertexPoolattach(copy._vertexPoolattach)
	{

	}

	IndexPool &IndexPool::operator=(IndexPool const &i)
	{
		if (this != &i)
		{
			clearPool();
			Pool::operator=(i);
			_vertexPoolattach = i._vertexPoolattach;
		}
		return (*this);
	}

	Pool &IndexPool::syncronisation()
	{
		if (!_syncronized || !_internalSyncronized)
			_ibo.bind();
		if (!_syncronized)
			_ibo.BufferData(_nbrBytePool);
		if (!_internalSyncronized)
		{
			if (_nbrAttribute)
				memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_nbrElementPool = 0;
			for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
			{
				Indices const *indices = index->second.data;
				MemoryBlocksGPU &memory = _poolMemory[index->second.memoryIndex];
				if (indices && memory.getSync() == false)
				{
					memory.setStartElement(_nbrElementPool);
					for (uint8_t index = 0; index < _nbrAttribute; ++index)
					{
						memory.setSync(true);
						memory.setOffset(index, _offsetAttribute[index] + _sizeAttribute[index]);
						_ibo.BufferSubData(memory.getOffset(index), memory.getSizeBlock(index), (void *)indices->getBuffer());
						_sizeAttribute[index] += memory.getSizeBlock(index);
					}
					_nbrElementPool += memory.getNbrElement();
				}
			}
		}
		_internalSyncronized = true;
		_syncronized = true;
		return (*this);
	}

	Buffer const &IndexPool::getBuffer() const
	{
		return (_ibo);
	}

	IndexPool const &IndexPool::onDrawCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target) const
	{
		Pool::Element<Indices> const *element;
		if ((element = getIndicesPoolElement(key, "rmIndices")) == NULL)
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->memoryIndex];
		glDrawElementsBaseVertex(mode, GLsizei(memory.getNbrElement()), GL_UNSIGNED_INT, (const GLvoid *)(memory.getElementStart() * sizeof(unsigned int)), GLint(target.getElementStart()));
		return (*this);
	}

	IndexPool const &IndexPool::onDrawIntancedCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target, size_t nbrIntanced) const
	{
		Pool::Element<Indices> const *element;
		if ((element = getIndicesPoolElement(key, "rmIndices")) == NULL)
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->memoryIndex];
		glDrawElementsInstancedBaseVertex(mode, GLsizei(memory.getNbrElement()), GL_UNSIGNED_INT, (const GLvoid *)(memory.getElementStart() * sizeof(unsigned int)), nbrIntanced, GLint(target.getElementStart()));
		return (*this);
	}

	IndexPool &IndexPool::attachVertexPoolToIndexPool(VertexPool const &pool)
	{
		_vertexPoolattach = &pool;
		return (*this);
	}

	IndexPool &IndexPool::dettachVertexPoolToIndexPool()
	{
		_vertexPoolattach = NULL;
		return (*this);
	}
}