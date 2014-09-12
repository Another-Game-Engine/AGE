#ifndef KEY_HH_
# define KEY_HH_

#include <iostream>

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
	template <typename TYPE>
	class Key
	{
	public:
		Key(int);
		Key();
		~Key();
		Key(Key<TYPE> const &copy);
		Key(Key<TYPE> &&copy);
		Key<TYPE> &operator=(Key<TYPE> const &t);
		Key<TYPE> &operator=(Key<TYPE> &&t);
		std::size_t getId() const;
		bool empty() const;
		bool operator!() const;
		bool operator==(Key<TYPE> const &compare) const;
		bool operator!=(Key<TYPE> const &compare) const;
		bool operator<(Key<TYPE> const &compare) const;
		bool operator>(Key<TYPE> const &compare) const;
		void destroy();

		////Hash function
		//struct hash_fn {
		//	size_t operator()(const Key<TYPE> &v) const
		//	{
		//		return v._id;
		//	}
		//};
	private:
		std::size_t _id;
	};
}

#include <Render/Key.hpp>

#endif /*KEY_HK_*/