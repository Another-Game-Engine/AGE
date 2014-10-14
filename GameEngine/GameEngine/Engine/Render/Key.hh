#ifndef KEY_HH_
# define KEY_HH_

#include <iostream>
#include <queue>
#include <cstdint>

#define KEY_DESTROY -1

# define CLASS_ACCESS \
	friend VerticesManager;\

namespace gl
{
	//!\file Key.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Key<TYPE>
	//!\brief Key use to represent opengl instance outside the render context
	template <typename TYPE, std::uint32_t POOL = 0>
	class Key
	{
	public:
		static Key<TYPE, POOL> createKey()
		{
			static size_t id = 0;
			if (!_trash.empty())
			{
				auto res = Key<TYPE, POOL>(_trash.front());
				_trash.pop();
				return res;
			}
			return (Key<TYPE, POOL>(++id));
		}

		// Use it with precautions
		static Key<TYPE, POOL> createKeyWithId(std::size_t _id)
		{
			return (Key<TYPE, POOL>(_id));
		}

		Key();
		~Key();
		Key(Key<TYPE, POOL> const &copy);
		Key<TYPE, POOL> &operator=(Key<TYPE, POOL> const &t);
		std::size_t getId() const;
		bool empty() const;
		bool operator!() const;
		bool operator==(Key<TYPE, POOL> const &compare) const;
		bool operator!=(Key<TYPE, POOL> const &compare) const;
		bool operator<(Key<TYPE, POOL> const &compare) const;
		bool operator>(Key<TYPE, POOL> const &compare) const;
		void destroy();

	private:
		std::size_t _id;
		explicit Key(size_t id);
		static std::queue<std::size_t> _trash;
	};
}

#include <Render/Key.hpp>

#endif /*KEY_HK_*/