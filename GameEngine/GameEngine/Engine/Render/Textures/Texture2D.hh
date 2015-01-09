#pragma once

# include <Render/Textures/ATexture.hh>
# include <utility>
# include <stdint.h>
# include <vector>

class Texture2D : public ATexture
{
public:
	Texture2D(GLint weight, GLint height, GLenum internal_format, bool is_mip_mapping);
	Texture2D(Texture2D &&move);

public:
	std::vector<uint8_t> &&get(GLint level, GLenum format, GLenum type) const;
	Texture2D &set(std::vector<uint8_t> const &data, GLint level, GLenum format, GLenum type);

public:
	virtual GLenum type() const override final;
	virtual ITexture const &bind() const override final;
	virtual ITexture const &unbind() const override final;
};