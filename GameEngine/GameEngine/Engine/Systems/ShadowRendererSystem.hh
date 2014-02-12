#ifndef SHADOWRENDERERSYSTEM_HH_
# define SHADOWRENDERERSYSTEM_HH_

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
		for (auto indice : _filter.getCollection())
		{
			auto &mesh = indice->getComponent<Component::ShadowRenderer>();
			mesh->render(_VPLight);
		}
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
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		_filter.require<Component::MeshRenderer>();
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 1, _height, _width, 0, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Somthing go wrong with the framebuffer" << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

private:
	glm::mat4 _VPLight;

};

# include "ShadowRendererSystem.hpp"

#endif /*!SHADOWRENDERERSYSTEM_HPP_*/