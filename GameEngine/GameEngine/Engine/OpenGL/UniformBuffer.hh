#ifndef		UNIFORMBUFFER_HH_
#define		UNIFORMBUFFER_HH_

namespace	OpenGLTools
{

template<size_t S = 4096>
class UniformBuffer
{
private:
	struct	SUniformVars
	{
		GLuint	size;
		GLuint	offset;
	};

	GLuint									_bindingPoint;
	GLuint									_bufferId;
	GLuint									_globalSize;
	std::map<std::string, SUniformVars>		_vars;
	char									_buffer[S];

public:
	UniformBuffer(GLuint bindingPoint);
	~UniformBuffer(void);

	void	init();
	void	registerUniform(std::string const &name, size_t offset, size_t size);
	void	setUniform(std::string const &name, void *data);

	void	flushChanges();

	GLuint	getBindingPoint() const;
};

#include "UniformBuffer.hpp"

}

#endif