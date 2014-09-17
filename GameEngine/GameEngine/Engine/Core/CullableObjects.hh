#pragma once

#include <Core/CullableInterfaces.hh>

namespace AGE
{
	typedef std::uint64_t DRAWABLE_ID;
	typedef std::uint64_t USER_OBJECT_ID;

	struct Mesh : public PreparableObject
	{
		AGE::Vector<DRAWABLE_ID> drawableCollection;
		USER_OBJECT_ID id;
	};

	struct Drawable : public CullableBoundingBox
	{
		DRAWABLE_ID id;
		SubMeshInstance mesh;
		MaterialInstance material;
		glm::mat4 transformation;
		// Cesar Bounding infos
		//		BoundingInfos boundingInfo;
		// This info must be filled by the asset manager
		AABoundingBox		meshAABB;

		Drawable();
		~Drawable();
		Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t);
		Drawable(SubMeshInstance &&_m, MaterialInstance &&_mat, glm::mat4 &&_t);
	};

	struct Camera : public CullableFrustum
	{
		glm::mat4 projection;
	};

	struct PointLight : public CullableSphere
	{
		float power;
		float range;
		glm::vec3 color;
		PointLight();
		PointLight(float power, float range, glm::vec3 const &color, glm::vec3 const &position);
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		AGE::Vector<Drawable> drawables;
		AGE::Vector<PointLight> lights;
	};
}