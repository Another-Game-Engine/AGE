#pragma once

# include <Render/Textures/ATexture.hh>
# include <utility>
# include <stdint.h>
# include <vector>
# include <Render/Pipelining/Buffer/AFramebufferStorage.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D : public ATexture, public AFramebufferStorage
	{
	public:
		Texture2D();
		virtual bool init(GLint weight, GLint height, GLenum internal_format, bool is_mip_mapping);
		Texture2D(Texture2D &&move);

	public:
		template <typename T>
		void get(GLint level, GLenum format, GLenum type, std::vector<T> &destination) const
		{
			glm::uvec2 mipmapSize = getMipmapSize(level);

			destination.resize(mipmapSize.x * mipmapSize.y);
			glGetTexImage(GL_TEXTURE_2D, level, format, type, (GLvoid *)destination.data());
		}

		glm::uvec2 getMipmapSize(GLint level) const;

		Texture2D &set(GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLenum type);
		Texture2D &setCompressed(GLvoid const *data, GLint level, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize);

	public:
		virtual GLenum type() const override final;
		virtual ITexture const &bind() const override final;
		virtual ITexture const &unbind() const override final;
		virtual ITexture const &parameter(GLenum mode, GLint param) const override final;
		virtual IFramebufferStorage const &attachment(Framebuffer const &framebuffer, GLenum attach) const override final;
		virtual void generateMipmaps() const override final;
	};
}