#include <Render/ProgramResources/Types/Uniform/Mat4Array255.hh>

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Debug.hpp>

namespace AGE
{
	Mat4Array255::Mat4Array255(GLint id, std::string &&name) :
		AProgramResources(id, std::move(name), GL_UNIFORM)
	{
		_size = 0;
	}

	Mat4Array255::Mat4Array255(Mat4Array255 &&move) :
		AProgramResources(std::move(move))
	{
		AGE_ASSERT("Not implemented");
	}

	Mat4Array255::Mat4Array255(Mat4Array255 const &copy) :
		AProgramResources(copy)
	{
		AGE_ASSERT("Not implemented");
	}

	void Mat4Array255::set(glm::mat4 *m, std::size_t size)
	{
		AGE_ASSERT(size <= 255);
		memcpy((void*)&_value, m, size * sizeof(glm::mat4));
		_size = size;
		_update = false;
	}

	IProgramResources & Mat4Array255::update()
	{
		if (!_update && _value) {
			glUniformMatrix4fv(_id, _size, GL_FALSE, (GLfloat *)(glm::value_ptr(_value[0])));
			_update = true;
		}
		return (*this);
	}

	bool Mat4Array255::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Mat4Array255::size() const
	{
		return (sizeof(type_t));
	}

	void Mat4Array255::print() const
	{
		std::cout << "uniform mat4 " << _name.str() << ";";
		std::cout << std::endl;
	}
}