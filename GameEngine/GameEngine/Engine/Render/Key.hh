#ifndef KEY_HH_
# define KEY_HH_

#include <iostream>

#define KEY_DESTROY -1

# define CLASS_ACCESS \
	friend VerticesManager;\

namespace gl
{
	template <typename TYPE>
	class Key
	{
	public:
		static Key<TYPE> createKey()
		{
			static size_t id = 0;
			return (Key<TYPE>(++id));
		}

		Key();
		~Key();
		Key(Key<TYPE> const &copy);
		Key<TYPE> &operator=(Key<TYPE> const &t);
		std::size_t getId() const;
		bool empty() const;
		bool operator!() const;
		bool operator==(Key<TYPE> const &compare) const;
		bool operator!=(Key<TYPE> const &compare) const;
		bool operator<(Key<TYPE> const &compare) const;
		bool operator>(Key<TYPE> const &compare) const;
		void destroy();

	private:
		std::size_t _id;
		explicit Key(size_t id);

	};
}

#include <Render/Key.hpp>

#endif /*KEY_HK_*/