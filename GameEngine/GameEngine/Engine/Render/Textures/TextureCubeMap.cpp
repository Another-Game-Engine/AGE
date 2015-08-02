#include <Render/Textures/TextureCubeMap.hh>
#include <glm/glm.hpp>

namespace AGE
{
	TextureCubeMap::TextureCubeMap() :
		ATexture(),
		_referenceTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X)
	{
	}

	bool TextureCubeMap::init(GLint width, GLint height, GLenum internal_format, bool is_mip_mapping)
	{
		auto nbr_mip_mapping = is_mip_mapping ? (uint8_t(glm::floor(glm::log2(glm::max(float(width), float(height))) + 1))) : 1;
		if (!ATexture::init(width, height, internal_format, nbr_mip_mapping)) {
			return false;
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, _nbr_mip_map, _internal_format, _width, _height);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return true;
	}

	TextureCubeMap::TextureCubeMap(TextureCubeMap &&move) :
		ATexture(std::move(move))
	{

	}

	GLenum TextureCubeMap::type() const
	{
		return (GL_TEXTURE_CUBE_MAP);
	}

	ITexture const & TextureCubeMap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
		return (*this);
	}

	ITexture const & TextureCubeMap::unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return (*this);
	}

	TextureCubeMap & TextureCubeMap::set(GLenum index, GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type)
	{
		glTexSubImage2D(index, level, 0, 0, width, height, format, type, data);
		return (*this);
	}

	TextureCubeMap & TextureCubeMap::setCompressed(GLenum index, GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize)
	{
		glCompressedTexSubImage2D(index, level, 0, 0, width, height, format, imageSize, data);
		return (*this);
	}

	ITexture const &TextureCubeMap::parameter(GLenum mode, GLint param) const
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, mode, param);
		return (*this);
	}

	TextureCubeMap &TextureCubeMap::referencedAttachmentFace(GLenum mode)
	{
		_referenceTexture = mode;
		return *this;
	}

	IFramebufferStorage const & TextureCubeMap::attachment(Framebuffer const &framebuffer, GLenum attach) const
	{
		bind();
		glFramebufferTexture2D(framebuffer.type(), attach, _referenceTexture, _id, 0);
		return (*this);
	}

	void TextureCubeMap::generateMipmaps() const
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

}