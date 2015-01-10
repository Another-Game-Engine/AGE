#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Pipelining/Buffer/Framebuffer.hh>

class IFramebufferStorage
{
public:
	virtual ~IFramebufferStorage(){}
	virtual IFramebufferStorage const &attachment(Framebuffer const &framebuffer, GLenum attach) const = 0;
};