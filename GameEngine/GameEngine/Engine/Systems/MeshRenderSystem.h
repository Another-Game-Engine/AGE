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
		//static double t = 0;
		//unsigned int textureOffset = 0;
		//auto &renderer = _scene->getEngine().getInstance<Renderer>();

		//t += time;

		//OpenGLTools::UniformBuffer *perModelUniform = _scene->getEngine().getInstance<Renderer>().getUniform("PerModel");

		//for (auto &mat : _sorted)
		//{
		//	for (auto &shaderName : mat.first->getShaders())
		//	{
		//		auto shader = _scene->getEngine().getInstance<Renderer>().getShader(shaderName);
		//		shader->use();
		//		for (auto &e : mat.second)
		//		{
		//			auto &mesh = e->getComponent<Component::MeshRenderer>();
		//			perModelUniform->setUniform("model", e->getGlobalTransform());
		//			perModelUniform->flushChanges();
		//			mesh->bindTextures(shader);
		//			mesh->getMesh()->draw();
		//		}
		//	}
		//}

		for (auto e : _collection)
		{
			auto &mesh = e->getComponent<Component::MeshRenderer>();
			mesh->render();
		}
	}

protected:

	std::map<SmartPointer<Material>, std::list<Handle> > _sorted;
	bool _renderDebugMethod;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
	}

	virtual void initialize()
	{
		require<Component::MeshRenderer>();
//		require<Component::MaterialComponent>();
		//globalSub("MaterialComponentChanged", [&](Handle &e, SmartPointer<Material> oldMaterial, SmartPointer<Material> newMaterial)
		//{
		//	if (_collection.find(e) == std::end(_collection) || oldMaterial == nullptr)
		//		return;
		//	if (_sorted.find(oldMaterial) != std::end(_sorted))
		//	{
		//		_sorted[oldMaterial].remove(e);
		//	}
		//	if (!newMaterial.get())
		//		return;
		//	_sorted[newMaterial].push_back(e);
		//});
	}

	//virtual void _componentAdded(Handle &e, unsigned int typeId)
	//{
	//	if (_code.match(e->getCode()) && _collection.find(e) == std::end(_collection))
	//	{
	//		auto m = e->getComponent<Component::MaterialComponent>()->getMaterial();
	//		if (_sorted.find(m) == std::end(_sorted))
	//		{
	//			_sorted.insert(std::make_pair(m, std::list<Handle>()));
	//		}
	//		_collection.insert(e);
	//		_sorted[e->getComponent<Component::MaterialComponent>()->getMaterial()].push_back(e);
	//	}
	//}

	//virtual void _componentRemoved(Handle &e, unsigned int typeId)
	//{
	//	if (!_code.match(e->getCode()) && _collection.find(e) != std::end(_collection))
	//	{
	//		_collection.erase(e);
	//		if (e->getComponent<Component::MaterialComponent>() != nullptr)
	//		{
	//			_sorted[e->getComponent<Component::MaterialComponent>()->getMaterial()].remove(e);
	//		}
	//	}
	//}
};

#endif    //__MESH_RENDERER_SYSTEM_HPP__