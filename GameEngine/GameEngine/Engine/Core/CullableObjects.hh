#pragma once

#include <Skinning/AnimationInstance.hpp>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <Utils/Frustum.hh>
#include <Utils/Sphere.hh>
#include <Utils/AABoundingBox.hh>
#include <SpacePartitioning/Cullable/CullableShape.hh>

namespace AGE
{
	typedef std::uint64_t DRAWABLE_ID;
	typedef std::uint64_t USER_OBJECT_ID;

	struct Mesh : public PreparableObject
	{
		AGE::Vector<DRAWABLE_ID> drawableCollection;
	};

	struct Drawable : public CullableShape<AABoundingBox>
	{
		SubMeshInstance mesh;
		glm::mat4 transformation;
		Key<AGE::AnimationInstance> animation;
		std::vector<glm::mat4> bones; // we'll have to find a optimized solution than copy vector of mat4 oO
		void reset();

		Drawable();
		virtual ~Drawable();
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t);
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t, const std::vector<glm::mat4> &_bones);
	};

	struct Camera : public CullableShape<Frustum>
	{
		uint32_t activeCameraIdx;

		glm::mat4 projection;
	};

	struct PointLight : public CullableShape<Sphere>
	{
		// To remove when light will be in octree
		uint32_t activePointLightIdx;

		glm::vec3 attenuation;
		glm::vec3 color;

		glm::mat4 sphereTransform;

		PointLight();
		PointLight(glm::vec3 const &position, glm::vec3 const &color, glm::vec3 const &attenuation);
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		AGE::Vector<Drawable> drawables;
		AGE::Vector<PointLight> lights;
	};
}