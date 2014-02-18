#ifndef SHADOWRENDERERSYSTEM_HH_
# define SHADOWRENDERERSYSTEM_HH_

# include "Utils/OpenGL.hh"
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include "System.h"
# include "Components/ShadowRenderer.hpp"
# include <Entities/EntityData.hh>
# include <Core/SceneManager.hh>

class ShadowRendererSystem : public System
{
public:
	ShadowRendererSystem(AScene *scene)
		: System(scene),
		_height(1000),
		_width(1000),
		_filter(scene)
	{
		glGenTextures(1, &_texture);
		glGenFramebuffers(1, &_frameBuffer);
	}

	virtual ~ShadowRendererSystem()
	{
		glDeleteTextures(1, &_texture);
		glDeleteFramebuffers(1, &_frameBuffer);
	}

	void render(double time)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		glDrawBuffer(GL_NONE);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (auto indice : _filter.getCollection())
		{
			auto &mesh = indice->getComponent<Component::ShadowRenderer>();
			mesh->render(_VPLight);
		}
		glFinish();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint getShadowMap() const
	{
		return (_texture);
	}

	void setLight(glm::mat4 const &VPLight)
	{
		_VPLight = VPLight;
	}

	glm::mat4 const &getLightVP()
	{
		return (_VPLight);
	}

	void clearLight()
	{
		_VPLight = glm::mat4(0);
	}

protected:
	GLuint _frameBuffer;
	uint32_t _height;
	uint32_t _width;
	GLuint _texture;
	EntityFilter _filter;

	virtual void updateBegin(double time){}
	virtual void updateEnd(double time){}
	virtual void mainUpdate(double time){}
	virtual void initialize()
	{
		_filter.require<Component::ShadowRenderer>();
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture, 0);
		GLenum mode = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (mode != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error frambuffer" << std::endl;
		if (mode == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			std::cout << "Error frambuffer incomplete attachment" << std::endl;
		if (mode == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			std::cout << "Error framebuffer missing attachement" << std::endl;
		if (mode == GL_FRAMEBUFFER_UNSUPPORTED)
			std::cout << "Error framebuffer unsupported" << std::endl;
	}

private:
	glm::mat4 _VPLight;

};

#endif /*!SHADOWRENDERERSYSTEM_HPP_*/