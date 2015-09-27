#pragma once
#include <cstdint>

namespace AGE
{
	typedef uint64_t ConcatenatedKey;

	template <typename TYPE>
	class Key
	{
	public:
		Key();

	public:
		static Key<TYPE> createKey(uint32_t index);

	public:
		uint32_t getId() const;
		bool isValid() const;
		void destroy();
		bool operator==(const Key &o);
		bool operator!=(const Key &o);
	private:
		uint32_t _id;
		explicit Key(uint32_t id);
	};

	template <typename type_t>
	inline uint32_t Key<type_t>::getId() const
	{
		return (_id);
	}

	template <typename type_t>
	inline bool Key<type_t>::isValid() const
	{
		return (_id != -1);
	}

	template <typename type_t>
	inline void Key<type_t>::destroy()
	{
		_id = -1;
	}

	template <typename type_t>
	inline Key<type_t>::Key(uint32_t id)
	{
		_id = id;
	}

	template <typename type_t>
	inline Key<type_t> Key<type_t>::createKey(uint32_t index)
	{
		return (Key<type_t>(index));
	}

	template <typename type_t>
	inline Key<type_t>::Key() :
		_id(-1)
	{

	}

	template <typename type_t>
	inline bool Key<type_t>::operator==(const Key<type_t> &o)
	{
		return (_id == o._id);
	}

	template <typename type_t>
	inline bool Key<type_t>::operator!=(const Key<type_t> &o)
	{
		return (_id != o._id);
	}

	template <typename T1, typename T2>
	static inline ConcatenatedKey ConcatenateKey(const Key<T1> &top, const Key<T2> &bottom)
	{
		return ((uint64_t)(top.getId()) << 32) | (uint64_t)(bottom.getId());
	}

	template <typename T1, typename T2>
	static inline void UnConcatenateKey(ConcatenatedKey concatenatedKey, Key<T1> &topResult, Key<T2> &bottomResult)
	{
		topResult = Key<T1>::createKey((uint32_t)(concatenatedKey >> 32));
		bottomResult = Key<T2>::createKey((uint32_t)(concatenatedKey));
	}
}