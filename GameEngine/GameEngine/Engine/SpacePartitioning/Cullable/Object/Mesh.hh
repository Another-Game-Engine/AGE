#pragma once

#include <AssetManagement/Instance/AnimationInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <SpacePartitioning/Cullable/CullableShape.hh>
#include <Utils/AABoundingBox.hh>
#include <vector>
#include <Render/Pipelining/Render/RenderModes.hh>

namespace AGE
{
	struct PointLight;
	class Properties;


	typedef std::uint64_t DRAWABLE_ID;
	typedef std::uint64_t USER_OBJECT_ID;

	struct Mesh : public PreparableObject
	{
		std::vector<DRAWABLE_ID> drawableCollection;
	};

	struct Drawable : public CullableShape<AABoundingBox>
	{
		SubMeshInstance mesh;
		Key<Property> transformationProperty;
		glm::mat4 transformation;
		Key<AnimationInstance> animation;
		std::vector<glm::mat4> bones; // we'll have to find a optimized solution than copy vector of mat4 oO
		std::vector < Key<Property> > materialKeys;
		RenderModeSet renderMode;
		void reset();

		Drawable();
		virtual ~Drawable();
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t);
		Drawable(const SubMeshInstance &_m, const glm::mat4 &_t, const std::vector<glm::mat4> &_bones);
	};

	struct DrawableCollection
	{
		glm::mat4 transformation;
		glm::mat4 projection;
		std::vector<Drawable> drawables;
		std::vector<PointLight> lights;
	};

}