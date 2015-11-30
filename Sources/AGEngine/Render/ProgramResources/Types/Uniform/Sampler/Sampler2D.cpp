#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <iostream>
#include <Render/Textures/Texture2D.hh>

namespace AGE
{
	Sampler2D::Sampler2D(GLint id, std::string &&name) :
		IProgramResources(id, std::move(name), GL_SAMPLER_2D),
		_texture(nullptr)
	{
		glUniform1i(_id, _id);
	}

	Sampler2D::Sampler2D(Sampler2D &&move) :
		IProgramResources(std::move(move)),
		_texture(std::move(_texture))
	{

	}

	Sampler2D::Sampler2D(Sampler2D const &copy) :
		IProgramResources(copy),
		_texture(copy._texture)
	{

	}

	Sampler2D::~Sampler2D()
	{

	}

	IProgramResources & Sampler2D::update()
	{
		if (!_update) {
			glActiveTexture(GL_TEXTURE0 + _id);
			if (_texture)
			{
				_texture->bind();
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			_update = true;
		}
		return (*this);
	}

	void Sampler2D::print() const
	{
		std::cout << "uniform sampler2D " << _name.str() << ";";
		std::cout << std::endl;
	}

	bool Sampler2D::safe(size_t s) const
	{
		return (size() == sizeof(s) ? true : false);
	}

	size_t Sampler2D::size() const
	{
		return (sizeof(type_t));
	}

	Sampler2D & Sampler2D::operator=(std::shared_ptr<Texture2D> const &texture)
	{
		_update = false;
		_texture = texture;
		return (*this);
	}
}