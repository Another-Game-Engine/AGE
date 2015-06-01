#pragma once

#include <Render/Textures/ATexture.hh>
#include <Render/Pipelining/Buffer/AFramebufferStorage.hh>
#include <vector>
#include <utility>
#include <stdint.h>

namespace AGE
{

	class Texture3D : public ATexture, public AFramebufferStorage
	{
	public:
		Texture3D();
		virtual bool init(GLint weight, GLint height, GLenum internal_format, bool is_mip_mapping);
		Texture3D(Texture3D &&move);

	public:
		Texture3D &set(GLenum index, std::vector<uint8_t> const &data, GLint level, GLenum format, GLenum type);
		Texture3D &referencedAttachmentFace(GLenum mode);

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