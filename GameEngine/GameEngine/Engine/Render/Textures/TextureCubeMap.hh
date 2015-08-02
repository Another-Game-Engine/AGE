#pragma once

#include <Render/Textures/ATexture.hh>
#include <Render/Pipelining/Buffer/AFramebufferStorage.hh>
#include <vector>
#include <utility>
#include <stdint.h>

namespace AGE
{

	class TextureCubeMap : public ATexture, public AFramebufferStorage
	{
	public:
		TextureCubeMap();
		virtual bool init(GLint weight, GLint height, GLenum internal_format, bool is_mip_mapping);
		TextureCubeMap(TextureCubeMap &&move);

	public:
		TextureCubeMap &set(GLenum index, GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type);
		TextureCubeMap &setCompressed(GLenum index, GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize);
		TextureCubeMap &referencedAttachmentFace(GLenum mode);

	public:
		virtual GLenum type() const override final;
		virtual ITexture const &bind() const override final;
		virtual ITexture const &unbind() const override final;
		virtual ITexture const &parameter(GLenum mode, GLint param) const override final;
		virtual void generateMipmaps() const override final;
		virtual IFramebufferStorage const &attachment(Framebuffer const &framebuffer, GLenum attach) const;

	private:
		GLenum _referenceTexture;
	};

}