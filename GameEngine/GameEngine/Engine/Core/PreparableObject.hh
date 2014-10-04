#pragma once

#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Core/PrepareKey.hpp>

namespace AGE
{
	typedef std::uint64_t DRAWABLE_ID;
	typedef std::uint64_t USER_OBJECT_ID;

	struct PreparableObject
	{
		bool active;
		bool hasMoved;
		PrepareKey key;
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat orientation;
		PreparableObject();
		~PreparableObject();
	};

	struct Mesh : public PreparableObject
	{
		AGE::Vector<DRAWABLE_ID> drawableCollection;
		USER_OBJECT_ID id;
	};

	struct Drawable : public PreparableObject
	{
		DRAWABLE_ID id;
		SubMeshInstance mesh;
		MaterialInstance material;
		glm::mat4 transformation;
		BoundingInfos boundingInfo;

		Drawable();
		~Drawable();
		Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t);
		Drawable(SubMeshInstance &&_m, MaterialInstance &&_mat, glm::mat4 &&_t);
	};

	struct Camera : public PreparableObject
	{
		glm::mat4 projection;
	};

	struct PointLight : public PreparableObject
	{
		glm::vec3 range;
		glm::vec3 color;
		PointLight();
		PointLight(glm::vec3 const &position, glm::vec3 const &color, glm::vec3 const &range);
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		AGE::Vector<Drawable> drawables;
		AGE::Vector<PointLight> lights;
	};
}