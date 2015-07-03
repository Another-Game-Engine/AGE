#pragma once

# include <Render/Textures/ITexture.hh>

namespace AGE
{
	class ATexture : public ITexture
	{
	protected:
		ATexture();
		ATexture(ATexture &&move);
		virtual bool init(GLint width, GLint height, GLenum internal_format, GLint nbr_mip_map);
	public:
		virtual ~ATexture();

	public:
		virtual GLuint id() const override final;
		virtual GLint width() const override final;
		virtual GLint height() const override final;
		virtual GLint nbrMipMap() const override final;
		virtual GLenum internal_format() const override final;

	protected:
		GLuint _id;
		GLint _width;
		GLint _height;
		GLint _internal_format;
		GLint _nbr_mip_map;
	};
}