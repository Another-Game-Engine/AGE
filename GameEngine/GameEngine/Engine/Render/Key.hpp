#ifdef KEY_HH_

namespace gl
{

	template <typename TYPE, std::uint32_t POOL = 0>
	std::queue<std::size_t> Key<TYPE, POOL>::_trash;

	template <typename TYPE, std::uint32_t POOL = 0>
	Key<TYPE, POOL>::Key()
	{
		_id = -1;
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	Key<TYPE, POOL>::~Key()
	{
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	Key<TYPE, POOL>::Key(Key<TYPE, POOL> const &copy) 
		: _id(copy._id)
	{
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	Key<TYPE, POOL> &Key<TYPE, POOL>::operator=(Key<TYPE, POOL> const &k)
	{
		_id = k._id;
		return (*this);
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	std::size_t Key<TYPE, POOL>::getId() const
	{
		return (_id);
	}

	// Test the validity of the key
	template <typename TYPE, std::uint32_t POOL = 0>
	bool Key<TYPE, POOL>::empty() const
	{
		return (_id == -1);
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	bool Key<TYPE, POOL>::operator!() const
	{
		return (empty());
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	bool Key<TYPE, POOL>::operator==(Key<TYPE, POOL> const &compare) const
	{
		return (_id == compare._id);
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	bool Key<TYPE, POOL>::operator!=(Key<TYPE, POOL> const &compare) const
	{
		return (!(*this == compare));
	}

	// render the key invalid to the use
	template <typename TYPE, std::uint32_t POOL = 0>
	void Key<TYPE, POOL>::destroy()
	{
		_id = -1;
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	bool Key<TYPE, POOL>::operator<(Key<TYPE, POOL> const &compare) const
	{
		return (_id < compare._id);
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	bool Key<TYPE, POOL>::operator>(Key<TYPE, POOL> const &compare) const
	{
		return (_id > compare._id);
	}

	template <typename TYPE, std::uint32_t POOL = 0>
	Key<TYPE, POOL>::Key(size_t id)
	{
		_id = id;
	}
}

#endif