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
		key(Key<TYPE> &&move);
		Key<TYPE> &operator=(Key<TYPE> const &t);
		std::size_t getId() const;
		bool empty() const;
	private:
		Key(std::size_t id);
		std::size_t _id;

		CLASS_ACCESS
	};
}

#include <OpenGL/Key.hpp>

#endif /*KEY_HK_*/