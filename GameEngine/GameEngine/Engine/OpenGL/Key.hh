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
		std::size_t getId(uint8_t index) const;
		uint8_t getCountId() const;
	private:
		std::size_t *_id;
		uint8_t _countId;
	};
}

#include <OpenGL/Key.hpp>

#endif /*KEY_HK_*/