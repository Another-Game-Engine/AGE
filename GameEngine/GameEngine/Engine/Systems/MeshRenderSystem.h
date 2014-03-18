#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Entities/EntityData.hh>
#include <Core/SceneManager.hh>

enum Shadow
{
	DIRECTIONAL = 0,
	POINT_LIGHT,
	UNKNOW
};

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(std::weak_ptr<AScene> scene)
		: System(scene),
		_type(UNKNOW),
		_shadow(false),
		_texShadow(0),
		_filter(scene),
		_renderDebugMethod(false)
	{
		glEnable(GL_TEXTURE_CUBE_MAP);
		_name = "mesh_renderer_system";
		glGenTextures(1, &_texShadow);
		glGenTextures(1, &_cubeMapShadow); 
		glGenFramebuffers(1, &_frameBuffer);
	}

	virtual ~MeshRendererSystem()
	{
		glDeleteTextures(1, &_texShadow);
		glDeleteTextures(1, &_cubeMapShadow);
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
			if (_type == DIRECTIONAL)
			{
				glBindTexture(GL_TEXTURE_2D, _texShadow);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texShadow, 0);
				GLenum mode = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (mode != GL_FRAMEBUFFER_COMPLETE)
					std::cout << "Error frambuffer" << std::endl;
				if (mode == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
					std::cout << "Error frambuffer incomplete attachment" << std::endl;
				if (mode == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
					std::cout << "Error framebuffer missing attachement" << std::endl;
				if (mode == GL_FRAMEBUFFER_UNSUPPORTED)
					std::cout << "Error framebuffer unsupported" << std::endl;
				for (auto e : _filter.getCollection())
				{
					auto &mesh = e->getComponent<Component::MeshRenderer>();
					mesh->render_shadowmap(_lightVP);
				}
			}
			else if (_type == POINT_LIGHT)
			{
				glm::vec3 cubeMapPos[6] =
				{
					glm::vec3(0, 1, 0),
					glm::vec3(1, 0, 0),
					glm::vec3(-1, 0, 0),
					glm::vec3(0, -1, 0),
					glm::vec3(0, 0, 1),
					glm::vec3(0, 0, -1)
				};

				glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapShadow);
				for (unsigned int j = 0; j < 6; ++j)
				{
					glm::mat4 viewProjection = glm::perspective<float>(90.0f, 1.0, 2.0f, 50.0f) * glm::lookAt(glm::vec3(0, 0, 0), cubeMapPos[j], glm::vec3(0, 1, 0));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, _cubeMapShadow, 0);
					GLenum mode = glCheckFramebufferStatus(GL_FRAMEBUFFER);
					if (mode != GL_FRAMEBUFFER_COMPLETE)
						std::cout << "Error frambuffer" << std::endl;
					if (mode == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
						std::cout << "Error frambuffer incomplete attachment" << std::endl;
					if (mode == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
						std::cout << "Error framebuffer missing attachement" << std::endl;
					if (mode == GL_FRAMEBUFFER_UNSUPPORTED)
						std::cout << "Error framebuffer unsupported" << std::endl;
					for (auto e : _filter.getCollection())
					{
						auto &mesh = e->getComponent<Component::MeshRenderer>();
						mesh->render_shadowmap(_lightVP);
					}
				}
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		for (auto e : _filter.getCollection())
		{	
			auto &mesh = e->getComponent<Component::MeshRenderer>();
			mesh->render(_shadow, _texShadow, _lightVP);
		}
	}

	void onShadow(Shadow type)
	{
		_type = type;
		_shadow = true;
	}

	void offShadow()
	{
		_shadow = false;
		_type = UNKNOW;
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
	Shadow _type;
	bool _shadow;
	glm::mat4 _lightVP;
	GLuint _texShadow;
	GLuint _cubeMapShadow;
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
		glBindTexture(GL_TEXTURE_2D, _texShadow);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapShadow);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		for (int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, 500, 500, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}


};

#endif    //__MESH_RENDERER_SYSTEM_HPP__