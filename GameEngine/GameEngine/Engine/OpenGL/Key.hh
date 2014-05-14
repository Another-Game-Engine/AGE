#ifndef KEY_HH_
# define KEY_HH_

#include <iostream>

# define CLASS_ACCESS \
	friend VerticesManager;\

namespace gl
{
	template <typename TYPE>
	class Key
	{
	public:
		~Key();
		Key(Key<TYPE> const &copy);
		Key<TYPE> &operator=(Key<TYPE> const &t);
		std::size_t getId() const;
		std::size_t getKey() const;
		bool empty() const;
		bool operator!() const;
		bool operator==(Key<TYPE> const &compare) const;
		bool operator!=(Key<TYPE> const &compare) const;
	private:
		Key(std::size_t id);
		std::size_t _id;
		std::size_t _key;

		CLASS_ACCESS
	};
}

#include <OpenGL/Key.hpp>

#endif /*KEY_HK_*/