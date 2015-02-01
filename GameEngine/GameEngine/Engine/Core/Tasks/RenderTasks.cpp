#include <Core/Tasks/RenderTasks.hpp>
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

		Render::SetMeshTransform::SetMeshTransform(Key<Painter> pPainter, Key<Vertices> pMesh,
			Key<Property> pTransform, glm::mat4 pTransformMat) :
			painter(pPainter),
			mesh(pMesh),
			transform(pTransform),
			transformMat(pTransformMat)
		{ }

	}
}