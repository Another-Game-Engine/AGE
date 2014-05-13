#ifdef KEY_HH_

namespace gl
{
	template <typename TYPE>
	Key<TYPE>::Key(std::size_t id)
		: _id(id),
	{
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
	Key<TYPE>::Key(Key<TYPE> &&move)
		: _id(std::move(move._id))
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
	bool Key<TYPE>::empty() const
	{
		if (_id == -1)
			return (true);
		else
			return (false);
	}
}

#endif