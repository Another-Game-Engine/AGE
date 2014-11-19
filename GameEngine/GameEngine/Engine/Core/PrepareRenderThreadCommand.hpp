#pragma once

#include "PrepareKey.hpp"
#include <glm/glm.hpp>
#include <Geometry/Material.hpp>
#include <Geometry/Mesh.hpp>
#include <Core/CullableObjects.hh>

namespace AGE
{
	namespace PRTC // Stand for Prepare Render Thread Commands
	{
		struct Position
		{
			Position(const PrepareKey &_key, const glm::vec3 &_position)
				: key(_key)
				, position(_position)
			{}
			PrepareKey key;
			glm::vec3 position;
		};

		struct Scale
		{
			Scale(const PrepareKey &_key, const glm::vec3 &_scale)
				: key(_key)
				, scale(_scale)
			{}
			PrepareKey key;
			glm::vec3 scale;
		};

		struct Orientation
		{
			Orientation(const PrepareKey &_key, const glm::quat &_orientation)
				: key(_key)
				, orientation(_orientation)
			{}
			PrepareKey key;
			glm::quat orientation;
		};

		struct Geometry
		{
			Geometry(const PrepareKey &_key
				, const AGE::Vector<SubMeshInstance> &_submeshInstances
				, const AGE::Vector<MaterialInstance> &_materialInstances
				, const gl::Key<AnimationInstance> &_animation)
				: key(_key)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
				, animation(_animation)
			{}
			PrepareKey key;
			AGE::Vector<SubMeshInstance> submeshInstances;
			AGE::Vector<MaterialInstance> materialInstances;
			gl::Key<AnimationInstance> animation;
		};

		struct CreateDrawable
		{
			CreateDrawable(const PrepareKey &_key)
				: key(_key)
			{}
			PrepareKey key;
		};

		struct DeleteDrawable
		{
			DeleteDrawable(const PrepareKey &_key)
				: key(_key)
			{}
			PrepareKey key;
		};

		struct CreateCamera
		{
			CreateCamera(const PrepareKey &_key)
				: key(_key)
			{}
			PrepareKey key;

		};

		struct CreatePointLight
		{
			CreatePointLight(PrepareKey const &key)
			: key(key)
			{}
			PrepareKey key;
		};

		struct SetPointLight
		{
			SetPointLight(glm::vec3 const &color, glm::vec3 const &range, PrepareKey &key)
			: key(key),
			color(color),
			range(range)
			{}
			PrepareKey key;
			glm::vec3 color;
			glm::vec3 range;
		};

		struct DeleteCamera
		{
			DeleteCamera(const PrepareKey &_key)
				: key(_key)
			{}
			PrepareKey key;
		};

		struct DeletePointLight
		{
			DeletePointLight(const PrepareKey &_key)
			: key(_key)
			{}
			PrepareKey key;
		};

		struct CameraInfos
		{
			CameraInfos(const PrepareKey &_key, const glm::mat4 &_projection)
				: key(_key)
				, projection(_projection)
			{}
			PrepareKey key;
			glm::mat4 projection;
		};

		struct PrepareDrawLists
		{
		};

		struct RenderDrawLists
		{
			std::function<void(DrawableCollection)> function;
			RenderDrawLists(std::function<void(DrawableCollection)> _function)
				: function(_function)
			{}
		};
	}
}