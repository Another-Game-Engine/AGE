#include <OpenGL/UniformBlock.hh>

namespace gl
{
	UniformBlock::UniformBlock()
		: _update(true),
		_data(NULL),
		_nbrElement(0),
		_sizeBlock(0)
	{
	}

	UniformBlock::~UniformBlock()
	{
		if (_data)
			delete[] _data;
	}

	UniformBlock::UniformBlock(size_t nbrElement, size_t *sizeElement)
		: _update(true),
		_data(NULL),
		_nbrElement(nbrElement)
	{
		static int bindingPoint = 0;
		_bindingPoint = bindingPoint;
		++bindingPoint;
		if (_nbrElement != 0)
			_data = new MemoryGPU[_nbrElement];
		for (size_t index = 0; index < _nbrElement; ++index)
		{
			_sizeBlock = 0;
			_data[index].size = sizeElement[index];
			_data[index].offset = _sizeBlock;
			_sizeBlock += _data[index].size;
		}
	}

	UniformBlock::UniformBlock(UniformBlock const &copy)
		: _update(true),
		_bindingPoint(copy._bindingPoint),
		_data(NULL),
		_nbrElement(copy._nbrElement),
		_sizeBlock(copy._sizeBlock)
	{
		if (_nbrElement != 0)
			_data = new MemoryGPU[_nbrElement];
		memcpy(&_data, &copy._data, sizeof(MemoryGPU)* _nbrElement);
	}

	UniformBlock &UniformBlock::operator=(UniformBlock const &u)
	{
		if (this != &u)
		{
			_update = true;
			_sizeBlock = _sizeBlock;
			_bindingPoint = u._bindingPoint;
			if (_nbrElement != u._nbrElement)
			{
				_nbrElement = u._nbrElement;
				if (_data)
					delete[] _data;
				if (_nbrElement != 0)
					_data = new MemoryGPU[_nbrElement];
				else
					_data = NULL;
			}
			memcpy(_data, u._data, sizeof(MemoryGPU)* _nbrElement);
		}
		return (*this);
	}

	size_t UniformBlock::getNbrElement() const
	{
		return (_nbrElement);
	}

	size_t UniformBlock::getSizeElement(size_t index) const
	{
		return (_data[index].size);
	}

	size_t UniformBlock::getOffsetElement(size_t index) const
	{
		return (_data[index].offset);
	}

	size_t UniformBlock::getSizeBlock(size_t index) const
	{
		return (_sizeBlock);
	}

	int UniformBlock::getBindingPoint() const
	{
		return (_bindingPoint);
	}

	void UniformBlock::GPUallocation()
	{
		_update = false;
		_ubo.bind();
		glBufferData(GL_UNIFORM_BUFFER, _sizeBlock, NULL, GL_STREAM_DRAW);
	}

	UniformBlock const &UniformBlock::bind() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _ubo.getId());
		return (*this);
	}

	UniformBlock const &UniformBlock::unbind() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, 0);
		return (*this);
	}
}