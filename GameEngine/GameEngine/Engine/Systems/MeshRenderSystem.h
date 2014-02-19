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
		_texShadow(0)
		, _filter(scene)
		, _renderDebugMethod(false)
	{
		std::array<Attribute, 4> param =
		{
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 2),
		};
		_manager = std::make_shared<VertexManager<4>>(param);
		_manager->init();
	}

	virtual ~MeshRendererSystem(){}

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
		for (auto e : _filter.getCollection())
		{
			auto &mesh = e->getComponent<Component::MeshRenderer>();
			mesh->render(_texShadow, _lightVP);
		}
	}

	void setTexShadow(GLuint texShadow)
	{
		_texShadow = texShadow;
	}

	void clearTexShadow()
	{
		_texShadow = 0;
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
	std::shared_ptr<VertexManager<4>> _manager;
	glm::mat4 _lightVP;
	GLuint _texShadow;
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
		for (auto collection : _filter.getCollection())
		{
			auto &object = collection->getComponent<Component::MeshRenderer>();
			object->initComponentManager(_manager);
		}
	}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__