#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Buffer/Framebuffer.hh>

class IStorages
{
public:
	virtual ~IStorages() {}
	virtual GLenum type() const = 0;
	virtual GLuint id() const = 0;
	virtual IStorages const &attachment(Framebuffer const &fbo, GLenum attach) const = 0;
};