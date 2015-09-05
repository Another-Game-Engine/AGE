#include <Render/Textures/Texture2D.hh>
#include <glm/glm.hpp>
#include <Render/Textures/PixelTypesFormats.hh>

namespace AGE
{
	Texture2D::Texture2D() :
		ATexture()
	{
	}

	bool Texture2D::init(GLint width, GLint height, GLenum internal_format, bool is_mip_mapping)
	{
		auto nbr_mip_mapping = is_mip_mapping ? (uint8_t(glm::floor(glm::log2(glm::max(float(width), float(height))) + 1))) : 1;
		if (!ATexture::init(width, height, internal_format, nbr_mip_mapping)) {
			return false;
		}
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexStorage2D(GL_TEXTURE_2D, _nbr_mip_map, _internal_format, _width, _height);
		return true;
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

	glm::uvec2 Texture2D::getMipmapSize(GLint level) const
	{
		glm::uvec2 mipMapSize(_width, _height);
		auto i = level;
		while (i > 0)
		{
			mipMapSize.x = (mipMapSize.x >> 1 != 0) ? (mipMapSize.x >> 1) : (1);
			mipMapSize.y = (mipMapSize.y >> 1 != 0) ? (mipMapSize.y >> 1) : (1);
			--i;
		}
		return (mipMapSize);
	}

	Texture2D & Texture2D::set(GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type)
	{
		glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, format, type, data);
		return (*this);
	}

	Texture2D &Texture2D::setCompressed(GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize)
	{
		glCompressedTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, format, imageSize, data);
		return (*this);
	}

	ITexture const &Texture2D::parameter(GLenum mode, GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, mode, param);
		return (*this);
	}

	IFramebufferStorage const & Texture2D::attachment(Framebuffer const &framebuffer, GLenum attach) const
	{
		bind();
		glFramebufferTexture2D(framebuffer.type(), attach, GL_TEXTURE_2D, _id, 0);
		return (*this);
	}

	void Texture2D::generateMipmaps() const
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

}