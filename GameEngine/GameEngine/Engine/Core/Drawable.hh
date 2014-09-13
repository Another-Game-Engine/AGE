#pragma once

#include <glm/fwd.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Render/Key.hh>

namespace gl
{
	class RenderPass;
}

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

	struct PointLight
	{
		float power;
		float range;
		glm::vec3 color;
		glm::vec4 position;
		PointLight(float power, float range, glm::vec3 const &color, glm::vec4 const &position)
			: power(power),
			range(range),
			color(color),
			position(position)
		{

		}
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		AGE::Vector<Drawable> drawables;
		AGE::Vector<PointLight> lights;
	};
}