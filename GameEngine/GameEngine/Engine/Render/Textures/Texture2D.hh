#pragma once

# include <Render/Textures/ATexture.hh>
# include <utility>
# include <stdint.h>
# include <vector>
# include <Render/Pipelining/Buffer/AFramebufferStorage.hh>

namespace AGE
{
	class Texture2D : public ATexture, public AFramebufferStorage
	{
	public:
		Texture2D();
		virtual bool init(GLint weight, GLint height, GLenum internal_format, bool is_mip_mapping);
		Texture2D(Texture2D &&move);

	public:
		std::vector<uint8_t> &&get(GLint level, GLenum format, GLenum type) const;
		Texture2D &set(std::vector<uint8_t> const &data, GLint level, GLenum format, GLenum type);

	public:
		virtual GLenum type() const override final;
		virtual ITexture const &bind() const override final;
		virtual ITexture const &unbind() const override final;
		virtual ITexture const &parameter(GLenum mode, GLint param) const override final;
		virtual IFramebufferStorage const &attachment(Framebuffer const &framebuffer, GLenum attach) const override final;
	};
}