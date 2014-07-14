#include "MeshRenderer.hh"
#include "Core/Engine.hh"
#include <Core/AScene.hh>	

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
		Cullable()
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	MeshRenderer::MeshRenderer(MeshRenderer &&o)
		: ComponentBase<MeshRenderer>(std::move(o))
		, Cullable(std::move(o))
	{
	}

	MeshRenderer &MeshRenderer::operator=(MeshRenderer &&o)
	{
		Cullable::operator=(std::move(o));
		return *this;
	}

	void MeshRenderer::init(AScene *scene, std::shared_ptr<ObjFile> r)
	{
		AGE::ComponentBehavior::Cullable::init(scene, entityId, MeshRenderer::getTypeId());
		setMesh(r);
	}

	void MeshRenderer::reset(AScene *scene)
	{
		AGE::ComponentBehavior::Cullable::reset(scene, entityId);
	}
}
