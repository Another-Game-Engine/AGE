#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/MaterialComponent.h>
#include <Entities/EntityData.hh>
#include <Core/SceneManager.hh>

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(AScene *scene)
		: System(scene)
		, _renderDebugMethod(false)
	{}
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
		for (auto e : _collection)
		{
			auto &mesh = e->getComponent<Component::MeshRenderer>();
			mesh->render();
		}
	}

protected:

	std::map<SmartPointer<Material>, std::list<Entity> > _sorted;
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
		require<Component::MeshRenderer>();
	}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__