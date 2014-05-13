#ifndef KEY_HH_
# define KEY_HH_

#include <iostream>

namespace gl
{
	template <typename TYPE>
	class Key
	{
	public:
		Key(std::size_t id);
		~Key();
		Key(Key<TYPE> const &copy);
		key(Key<TYPE> &&move);
		Key<TYPE> &operator=(Key<TYPE> const &t);
		std::size_t getId() const;
		bool empty();
	private:
		std::size_t _id;
	};
}

#include <OpenGL/Key.hpp>

#endif /*KEY_HK_*/