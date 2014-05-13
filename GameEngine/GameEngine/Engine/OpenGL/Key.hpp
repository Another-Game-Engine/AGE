#ifdef KEY_HH_

namespace gl
{
	template <typename TYPE>
	Key<TYPE>::Key(std::size_t *id, uint8_t countId)
		: _id(new std::size[countId]),
		_countId(countId)
	{
		memcpy(_id, id, sizeof(std::size_t) * countId);
	}

	template <typename TYPE>
	Key<TYPE>::~Key()
	{
		if (_id)
			delete[] _id;
	}

	template <typename TYPE>
	Key<TYPE>::Key(Key<TYPE> const &copy) 
		: _id(new std::size_t[copy._countId]),
		_countId(copy._countId)
	{
		memcpy(_id, copy._id, sizeof(std::size_t) * copy._countId);
	}

	template <typename TYPE>
	Key<TYPE>::Key(Key<TYPE> &&move)
		: _id(std::move(move._id)),
		_countId(std::move(move._countId))
	{
	}

	template <typename TYPE>
	Key<TYPE> &Key<TYPE>::operator=(Key<TYPE> const &k)
	{
		memcpy(_id, k._id, sizeof(std::size_t) * k._countId);
		_countId = k._countId;
		return (*this);
	}

	template <typename TYPE>
	std::size_t Key<TYPE>::getId(uint8_t index) const
	{
		if (index >= _countId)
			_return (-1);
		return (_id[index]);
	}

	template <typename TYPE>
	uint8_t Key<TYPE>::getCountId() const
	{
		return (_countId);
	}
}

#endif