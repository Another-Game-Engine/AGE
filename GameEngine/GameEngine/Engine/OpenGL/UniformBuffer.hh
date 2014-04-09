#ifndef		UNIFORMBUFFER_HH_
#define		UNIFORMBUFFER_HH_

#include "Utils/OpenGL.hh"
#include "glm/glm.hpp"
#include <memory>
#include <map>

namespace	OpenGLTools
{

class Shader;

class UniformBuffer
{
private:
	struct	SUniformVars
	{
		GLuint	offset;
		GLint	type;

		SUniformVars()
		{
		}

		SUniformVars(SUniformVars const &oth)
		{
			*this = oth;
		}

		SUniformVars	&operator=(SUniformVars const &oth)
		{
			type = oth.type;
			offset = oth.offset;
			return (*this);
		}
	};

	GLuint									_bindingPoint;
	GLuint									_bufferId;
	std::uint32_t							_dataSize;
	std::map<std::string, SUniformVars>		_vars;
	char									*_buffer;

public:
	UniformBuffer(GLuint bindingPoint);
	~UniformBuffer(void);


	void			init(std::shared_ptr<Shader> referent, std::string const &blockName, std::uint32_t size);
	void			init(std::shared_ptr<Shader> referent, std::string const &blockName, std::string const vars[]);

	void			setBufferData(size_t size, const char *data);

	void			setUniform(std::string const &name, int data);
	void			setUniform(std::string const &name, unsigned int data);
	void			setUniform(std::string const &name, float data);
	void			setUniform(std::string const &name, glm::vec2 const &data);
	void			setUniform(std::string const &name, glm::vec3 const &data);
	void			setUniform(std::string const &name, glm::vec4 const &data);
	void			setUniform(std::string const &name, glm::mat2 const &data);
	void			setUniform(std::string const &name, glm::mat3 const &data);
	void			setUniform(std::string const &name, glm::mat4 const &data);

	void	flushChanges();

	GLuint	getBindingPoint() const;
};

}

#endif