#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler3D.hh>
#include <iostream>
#include <Render/Textures/TextureCubeMap.hh>

namespace AGE
{
	Sampler3D::Sampler3D(GLint id, std::string &&name) :
		AProgramResources(id, std::move(name), GL_SAMPLER_CUBE),
		_texture(nullptr)
	{
		glUniform1i(_id, _id);
	}

	Sampler3D::Sampler3D(Sampler3D &&move) :
		AProgramResources(std::move(move)),
		_texture(std::move(_texture))
	{

	}

	Sampler3D::Sampler3D(Sampler3D const &copy) :
		AProgramResources(copy),
		_texture(copy._texture)
	{

	}

	IProgramResources & Sampler3D::update()
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

	void Sampler3D::print() const
	{
		std::cout << "uniform sampler3D " << _name.str() << ";";
		std::cout << std::endl;
	}

	bool Sampler3D::safe(size_t s) const
	{
		return (size() == sizeof(s) ? true : false);
	}

	size_t Sampler3D::size() const
	{
		return (sizeof(type_t));
	}

	Sampler3D & Sampler3D::operator=(std::shared_ptr<TextureCubeMap> const &texture)
	{
		_update = false;
		_texture = texture;
		return (*this);
	}
}