#include "MainToPrepare.hpp"
#include <Utils/Containers/Vector.hpp>
#include <Geometry/Mesh.hpp>
#include <Core/CullableObjects.hh>

namespace AGE
{
	namespace Commands
	{
		namespace MainToPrepare
		{
			SetPosition::SetPosition(const PrepareKey &_key, const glm::vec3 &_position)
				: key(_key)
				, position(_position)
			{}

			SetScale::SetScale(const PrepareKey &_key, const glm::vec3 &_scale)
				: key(_key)
				, scale(_scale)
			{}

			SetOrientation::SetOrientation(const PrepareKey &_key, const glm::quat &_orientation)
				: key(_key)
				, orientation(_orientation)
			{}


			SetGeometry::SetGeometry(const PrepareKey &_key
				, const AGE::Vector<SubMeshInstance> &_submeshInstances
				, const AGE::Vector<MaterialInstance> &_materialInstances
				, const gl::Key<AnimationInstance> &_animation)
				: key(_key)
				, submeshInstances(_submeshInstances)
				, materialInstances(_materialInstances)
				, animation(_animation)
			{
			}

			CreateDrawable::CreateDrawable(const PrepareKey &_key)
				: key(_key)
			{}

			DeleteDrawable::DeleteDrawable(const PrepareKey &_key)
				: key(_key)
			{}

			CreateCamera::CreateCamera(const PrepareKey &_key)
				: key(_key)
			{}

			CreatePointLight::CreatePointLight(PrepareKey const &key)
				: key(key)
			{}

			SetPointLight::SetPointLight(glm::vec3 const &color, glm::vec3 const &range, PrepareKey &key)
				: key(key),
				color(color),
				range(range)
			{}

			DeleteCamera::DeleteCamera(const PrepareKey &_key)
				: key(_key)
			{}

			DeletePointLight::DeletePointLight(const PrepareKey &_key)
				: key(_key)
			{}

			CameraInfos::CameraInfos(const PrepareKey &_key, const glm::mat4 &_projection)
				: key(_key)
				, projection(_projection)
			{}

			SceneUpdateBegin::SceneUpdateBegin(AScene *_scene)
				: scene(_scene)
			{
			}
		}
	}
}