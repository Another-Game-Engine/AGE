#include "MeshRenderer.hh"
#include "Core/Engine.hh"
#include <Core/AScene.hh>
#include <Geometry/Mesh.hpp>

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

	void MeshRenderer::init(AScene *scene, std::shared_ptr<AGE::MeshInstance> r)
	{
		AGE::ComponentBehavior::Cullable::init(scene, entityId);
		setMesh(r);
	}

	void MeshRenderer::reset(AScene *scene)
	{
		AGE::ComponentBehavior::Cullable::reset(scene, entityId);
	}
}
