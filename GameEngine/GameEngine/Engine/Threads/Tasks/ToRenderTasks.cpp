#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Engine/Render/Properties/Properties.hh>
#include <glm/glm.hpp>

namespace AGE
{
	namespace Tasks
	{
		Render::GetWindowSize::GetWindowSize(){}
		Render::SetWindowSize::SetWindowSize(const glm::uvec2 & _size)
			: size(_size)
		{}
		Render::CreateRenderContext::CreateRenderContext(std::weak_ptr<Engine> _engine) : engine(_engine){}

		Render::RemoveMeshProperty::RemoveMeshProperty(Key<Properties> const &pToRemove) :
			toRemove(pToRemove)
		{
		}

		Render::SetMeshTransform::SetMeshTransform(Key<Properties> pMeshProperties,
			Key<Property> pTransformProperty,
			glm::mat4 pTransformMat) :
			meshProperties(pMeshProperties),
			transformProperty(pTransformProperty),
			transformMat(pTransformMat)
		{ }

	}
}