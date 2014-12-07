#include "MainToPrepare.hpp"


namespace AGE
{
	namespace Commands
	{
		MainToPrepare::SetPosition::SetPosition(const PrepareKey &_key, const glm::vec3 &_position)
			: key(_key)
			, position(_position)
		{}

		MainToPrepare::SetScale::SetScale(const PrepareKey &_key, const glm::vec3 &_scale)
			: key(_key)
			, scale(_scale)
		{}

		MainToPrepare::SetOrientation::SetOrientation(const PrepareKey &_key, const glm::quat &_orientation)
			: key(_key)
			, orientation(_orientation)
		{}


		MainToPrepare::SetGeometry::SetGeometry(const PrepareKey &_key
			, const AGE::Vector<SubMeshInstance> &_submeshInstances
			, const AGE::Vector<MaterialInstance> &_materialInstances
			, const gl::Key<AnimationInstance> &_animation)
			: key(_key)
			, submeshInstances(_submeshInstances)
			, materialInstances(_materialInstances)
			, animation(_animation)
		{}

		MainToPrepare::CreateDrawable::CreateDrawable(const PrepareKey &_key)
			: key(_key)
		{}

		MainToPrepare::DeleteDrawable::DeleteDrawable(const PrepareKey &_key)
			: key(_key)
		{}

		MainToPrepare::CreateCamera::CreateCamera(const PrepareKey &_key)
			: key(_key)
		{}

		MainToPrepare::CreatePointLight::CreatePointLight(PrepareKey const &key)
			: key(key)
		{}

		MainToPrepare::SetPointLight::SetPointLight(glm::vec3 const &color, glm::vec3 const &range, PrepareKey &key)
			: key(key),
			color(color),
			range(range)
		{}

		MainToPrepare::DeleteCamera::DeleteCamera(const PrepareKey &_key)
			: key(_key)
		{}

		MainToPrepare::DeletePointLight::DeletePointLight(const PrepareKey &_key)
			: key(_key)
		{}

		MainToPrepare::CameraInfos::CameraInfos(const PrepareKey &_key, const glm::mat4 &_projection)
			: key(_key)
			, projection(_projection)
		{}

		MainToPrepare::SceneUpdateBegin::SceneUpdateBegin(std::size_t _sceneId)
			: sceneId(_sceneId)
		{
		}
	}
}