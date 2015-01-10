#pragma once

#include <Core/CullableInterfaces.hh>
#include <Skinning/AnimationInstance.hpp>

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
		glm::mat4 transformation;
		AABoundingBox		meshAABB;
		Key<AGE::AnimationInstance> animation;
		std::vector<glm::mat4> bones; // we'll have to find a optimized solution than copy vector of mat4 oO
		void reset();

		Drawable();
		virtual ~Drawable();
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t);
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t, const std::vector<glm::mat4> &_bones);
	};

	struct Camera : public CullableFrustum
	{
		glm::mat4 projection;
	};

	struct PointLight : public CullableSphere
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