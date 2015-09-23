#pragma once

# include <Render/Textures/ATexture.hh>
# include <Render/Pipelining/Buffer/AFramebufferStorage.hh>

namespace AGE
{

	class Renderbuffer : public AFramebufferStorage
	{
	public:
		Renderbuffer();
		Renderbuffer(Renderbuffer &&move);
		~Renderbuffer();

	public:
		GLenum type() const;
		Renderbuffer const &bind() const;
		Renderbuffer const &unbind() const;
		GLint width() const;
		GLint height() const;
		bool init(GLint width, GLint height, GLenum internal_format);

	public:
		virtual IFramebufferStorage const &attachment(Framebuffer const &framebuffer, GLenum attach) const override;

	private:
		GLuint _id;
		GLint _width;
		GLint _height;
		GLenum _internal_format;
	};

}