#pragma once

#include <glm/glm.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Utils/Containers/Vector.hpp>

namespace AGE
{
	struct Drawable
	{
		SubMeshInstance mesh;
		MaterialInstance material;
		glm::mat4 transformation;

		Drawable();
		~Drawable();
		Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t);
		Drawable(SubMeshInstance &&_m, MaterialInstance &&_mat, glm::mat4 &&_t);
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		AGE::Vector<Drawable> drawables;
	};
}