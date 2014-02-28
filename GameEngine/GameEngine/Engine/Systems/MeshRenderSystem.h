#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Entities/EntityData.hh>
#include <Core/SceneManager.hh>

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(AScene *scene)
		: System(scene),
		_shadow(false),
		_texShadow(0),
		_filter(scene),
		_renderDebugMethod(false)
	{
		glGenTextures(1, &_texShadow);
		glGenFramebuffers(1, &_frameBuffer);
	}

	virtual ~MeshRendererSystem()
	{
		glDeleteTextures(1, &_texShadow);
		glDeleteFramebuffers(1, &_frameBuffer);
	}

	void setRenderDebugMode(bool t)
	{
		_renderDebugMethod = t;
	}

	bool getRenderDebugMode() const
	{
		return _renderDebugMethod;
	}

	void render(double time)
	{
		if (_shadow)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
			glDrawBuffer(GL_NONE);
			glClearDepth(1.0f);
			glDepthFunc(GL_LESS);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (auto e : _filter.getCollection())
			{
				auto &mesh = e->getComponent<Component::MeshRenderer>();
				mesh->render_shadowmap(_texShadow, _lightVP);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		for (auto e : _filter.getCollection())
		{	
			auto &mesh = e->getComponent<Component::MeshRenderer>();
			mesh->render(_shadow, _texShadow, _lightVP);
		}
		glFinish();
	}

	void onShadow()
	{
		_shadow = true;
	}

	void offShadow()
	{
		_shadow = false;
	}

	void setLightVP(glm::mat4 const &lightVP)
	{
		_lightVP = lightVP;
	}

	void clearLightVP()
	{
		_lightVP = glm::mat4(0);
	}

protected:
	bool _shadow;
	glm::mat4 _lightVP;
	GLuint _texShadow;
	GLuint _frameBuffer;
	EntityFilter _filter;
	bool _renderDebugMethod;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{
	}

	virtual void mainUpdate(double time)
	{
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::MeshRenderer>();
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		glBindTexture(GL_TEXTURE_2D, _texShadow);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _texShadow, 0);
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


};

#endif    //__MESH_RENDERER_SYSTEM_HPP__