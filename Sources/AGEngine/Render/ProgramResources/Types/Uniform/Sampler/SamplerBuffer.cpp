#include <Render/ProgramResources/Types/Uniform/Sampler/SamplerBuffer.hh>
#include <Render/Textures/TextureBuffer.hh>

#include <iostream>

namespace AGE
{
	SamplerBuffer::SamplerBuffer(GLint id, std::string &&name) :
		IProgramResources(id, std::move(name), GL_SAMPLER_BUFFER),
		_texture(nullptr)
	{
		glUniform1i(_id, _id);
	}

	SamplerBuffer::SamplerBuffer(SamplerBuffer &&move) :
		IProgramResources(std::move(move)),
		_texture(std::move(_texture))
	{

	}

	SamplerBuffer::SamplerBuffer(SamplerBuffer const &copy) :
		IProgramResources(copy),
		_texture(copy._texture)
	{

	}

	SamplerBuffer::~SamplerBuffer()
	{

	}

	IProgramResources & SamplerBuffer::update()
	{
		if (!_update)
		{
			glActiveTexture(GL_TEXTURE0 + _id);
			if (_texture)
			{
				_texture->bind();
			}
			else
			{
				glBindTexture(GL_TEXTURE_BUFFER, 0);
			}
			_update = true;
		}
		return (*this);
	}

	void SamplerBuffer::print() const
	{
		std::cout << "uniform SamplerBuffer " << _name.str() << ";";
		std::cout << std::endl;
	}

	bool SamplerBuffer::safe(size_t s) const
	{
		return (size() == sizeof(s) ? true : false);
	}

	size_t SamplerBuffer::size() const
	{
		return (sizeof(type_t));
	}

	SamplerBuffer & SamplerBuffer::operator=(std::shared_ptr<TextureBuffer> const &texture)
	{
		_update = false;
		_texture = texture;
		return (*this);
	}
}