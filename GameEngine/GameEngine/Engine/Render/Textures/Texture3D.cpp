#include <Render/Textures/Texture3D.hh>
#include <glm/glm.hpp>
#include <Render/Textures/PixelTypesFormats.hh>

namespace AGE
{
	Texture3D::Texture3D() :
		ATexture()
	{
	}

	bool Texture3D::init(GLint width, GLint height, GLenum internal_format, bool is_mip_mapping)
	{
		auto nbr_mip_mapping = is_mip_mapping ? (uint8_t(glm::floor(glm::log2(glm::max(float(width), float(height))) + 1))) : 1;
		if (!ATexture::init(width, height, internal_format, nbr_mip_mapping)) {
			return false;
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, _nbr_mip_map, _internal_format, _width, _height);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, _nbr_mip_map, _internal_format, _width, _height);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, _nbr_mip_map, _internal_format, _width, _height);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, _nbr_mip_map, _internal_format, _width, _height);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, _nbr_mip_map, _internal_format, _width, _height);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, _nbr_mip_map, _internal_format, _width, _height);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return true;
	}

	Texture3D::Texture3D(Texture3D &&move) :
		ATexture(std::move(move))
	{

	}

	GLenum Texture3D::type() const
	{
		return (GL_TEXTURE_CUBE_MAP);
	}

	ITexture const & Texture3D::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
		return (*this);
	}

	ITexture const & Texture3D::unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return (*this);
	}

	Texture3D & Texture3D::set(GLenum index, std::vector<uint8_t> const &data, GLint level, GLenum format, GLenum type)
	{
		glTexSubImage2D(index, level, 0, 0, _width, _height, format, type, (GLvoid const *)data.data());
		return (*this);
	}

	ITexture const &Texture3D::parameter(GLenum mode, GLint param) const
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, mode, param);
		return (*this);
	}

	void Texture3D::generateMipmaps() const
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

}