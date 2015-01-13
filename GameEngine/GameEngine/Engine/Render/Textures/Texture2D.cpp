#include <Render/Textures/Texture2D.hh>
#include <glm/glm.hpp>
#include <Render/Textures/PixelTypesFormats.hh>

Texture2D::Texture2D(GLint width, GLint height, GLenum internal_format, bool is_mip_mapping) :
ATexture(width, height, internal_format, is_mip_mapping ? (uint8_t(glm::floor(glm::log2(glm::max(float(width), float(height))) + 1))) : 1)
{
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexStorage2D(GL_TEXTURE_2D, _nbr_mip_map, _internal_format, _width, _height);
}

Texture2D::Texture2D(Texture2D &&move) :
ATexture(std::move(move))
{

}

GLenum Texture2D::type() const
{
	return (GL_TEXTURE_2D);
}

ITexture const & Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, _id);
	return (*this);
}

ITexture const & Texture2D::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	return (*this);
}

std::vector<uint8_t> &&Texture2D::get(GLint level, GLenum format, GLenum type) const
{
	std::vector<uint8_t> data(_width * _height * available_type_pixel[type].size);
	glGetTexImage(GL_TEXTURE_2D, level, format, type, (GLvoid *)data.data());
	return (std::move(data));
}

Texture2D & Texture2D::set(std::vector<uint8_t> const &data, GLint level, GLenum format, GLenum type)
{
	glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, _width, _height, format, type, (GLvoid const *)data.data());
	return (*this);
}

ITexture const &Texture2D::parameter(GLenum mode, GLint param) const
{
	glTexParameteri(GL_TEXTURE_2D, mode, param);
	return (*this);
}

IFramebufferStorage const & Texture2D::attachment(Framebuffer const &framebuffer, GLenum attach) const
{
	glFramebufferTexture2D(framebuffer.type(), attach, GL_TEXTURE_2D, _id, 0);
	return (*this);
}
