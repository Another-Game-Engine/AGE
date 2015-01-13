#pragma once

# include <Render/Textures/ATexture.hh>
# include <Render/Pipelining/Buffer/AFramebufferStorage.hh>

class Renderbuffer : public AFramebufferStorage
{
public:
	Renderbuffer(GLint width, GLint height, GLenum internal_format);
	Renderbuffer(Renderbuffer &&move);
	~Renderbuffer();

public:
	GLenum type() const;
	Renderbuffer const &bind() const;
	Renderbuffer const &unbind() const;
	GLint width() const;
	GLint height() const;

public:
	virtual IFramebufferStorage const &attachment(Framebuffer const &framebuffer, GLenum attach) const override;

private:
	GLuint _id;
	GLint _width;
	GLint _height;
	GLenum _internal_format;
};