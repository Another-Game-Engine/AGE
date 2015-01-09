#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Buffer/Framebuffer.hh>

class ITexture
{
public:
	virtual ~ITexture() {}
	virtual GLenum type() const = 0;
	virtual ITexture const &bind() const = 0;
	virtual ITexture const &unbind() const = 0;
	virtual GLuint id() const = 0;
	virtual GLint width() const = 0;
	virtual GLint height() const = 0;
	virtual GLenum internal_format() const = 0;
};