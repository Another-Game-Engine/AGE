#ifdef KEY_HH_

namespace gl
{
	template <typename TYPE>
	Key<TYPE>::Key(std::size_t id)
		: _id(id),
	{
		static size_t key = 0;
		_key = ++key;
	}

	template <typename TYPE>
	Key<TYPE>::~Key()
	{
	}

	template <typename TYPE>
	Key<TYPE>::Key(Key<TYPE> const &copy) 
		: _id(copy._id)
	{
	}

	template <typename TYPE>
	Key<TYPE> &Key<TYPE>::operator=(Key<TYPE> const &k)
	{
		_id = k._id;
		return (*this);
	}

	template <typename TYPE>
	std::size_t Key<TYPE>::getId() const
	{
		return (_id);
	}

	template <typename TYPE>
	std::size_t Key<TYPE>::getKey() const
	{
		return (_key);
	}

	template <typename TYPE>
	bool Key<TYPE>::empty() const
	{
		if (_key == -1)
			return (true);
		else
			return (false);
	}

	template <typename TYPE>
	bool Key<TYPE>::operator!() const
	{
		return (empty());
	}

	template <typename TYPE>
	bool Key<TYPE>::operator==(Key<TYPE> const &compare) const
	{
		if (_key == compare._key)
			return (true);
		return (false);
	}

	template <typename TYPE>
	bool Key<TYPE>::operator!=(Key<TYPE> const &compare) const
	{
		return (!(*this == compare));
	}
}

#endif