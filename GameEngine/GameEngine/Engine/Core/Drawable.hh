#pragma once

#include <glm/glm.hpp>
#include <Geometry/Mesh.hpp>
#include <Utils/Containers/Queue.hpp>

namespace AGE
{
	struct Drawable
	{
		SubMeshInstance mesh;
		glm::mat4 transformation;

		Drawable();
		~Drawable();
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t);
		Drawable(SubMeshInstance &&_m, glm::mat4 &&_t);
		Drawable &operator=(Drawable const &d);
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		AGE::Queue<Drawable> drawables;
	};
}