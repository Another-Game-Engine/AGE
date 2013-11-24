#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Entities/Entity.hh>

bool meshRendererComparaison(Entity *e1, Entity *e2)
{
	auto m1 = e1->getComponent<Component::MeshRenderer>();
	auto m2 = e2->getComponent<Component::MeshRenderer>();

	if (!m1.get() || !m2.get())
		return false;
	return std::strcmp(m1->getName().c_str(), m2->getName().c_str()) < 0;
}

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem()
		: System(meshRendererComparaison)
	{}
	virtual ~MeshRendererSystem(){}
private:
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{

	}

	virtual void initialize()
	{
		require<Component::MeshRenderer>();
	}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__