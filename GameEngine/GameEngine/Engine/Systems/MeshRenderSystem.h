#ifndef   __MESH_RENDERER_SYSTEM_HPP__
# define  __MESH_RENDERER_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/MeshRenderer.hh>
#include <Components/MaterialComponent.h>
#include <Entities/Entity.hh>

bool meshRendererComparaison(Entity *e1, Entity *e2)
{
	auto m1 = e1->getComponent<Component::ComponentMaterial>();
	auto m2 = e2->getComponent<Component::ComponentMaterial>();

	if (!m1.get() || !m2.get())
		return false;
	return std::strcmp(m1->getMaterial()->getName().c_str(), m2->getMaterial()->getName().c_str()) < 0;
}

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem()
		: System(meshRendererComparaison)
	{}
	virtual ~MeshRendererSystem(){}
private:

	std::map<Material*, std::list<Entity*> > _sorted;

	virtual void updateBegin(double time)
	{
		_sorted.clear();
		for (auto &e : _collection)
		{
			auto m = e->getComponent<Component::ComponentMaterial>()->getMaterial().get();
			_sorted[m].push_back(e);
		}
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto &mat : _sorted)
		{
			for (auto &shaderName : mat.first->getShaders())
			{
				auto shader = GameEngine::instance()->renderer().getShader(shaderName);
				shader->use();
				for (auto &e : mat.second)
				{
					auto &mesh = e->getComponent<Component::MeshRenderer>();
					GameEngine::instance()->renderer().getUniform("PerModel")->setUniform("model", e->getGlobalTransform());
					GameEngine::instance()->renderer().getUniform("PerModel")->flushChanges();
					mesh->bindTextures(shader);
					mesh->getMesh()->draw();
					mesh->unbindTextures();
				}
			}
		}
	}

	virtual void initialize()
	{
		require<Component::MeshRenderer>();
		require<Component::ComponentMaterial>();
		sub("MaterialComponentChanged", [&](Entity *e){
			_collection.erase(e);
			_collection.insert(e);
		});
	}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__