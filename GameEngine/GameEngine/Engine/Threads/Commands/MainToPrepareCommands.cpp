#include <Threads/Commands/MainToPrepareCommands.hpp>
#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AGE
{
	namespace Commands
	{
		namespace MainToPrepare
		{
			SetTransform::SetTransform(const PrepareKey &_key, const glm::mat4 &_transform)
				: key(_key)
				, transform(_transform)
			{}

			SetGeometry::SetGeometry(const PrepareKey &_key
				, const AGE::Vector<SubMeshInstance> &_submeshInstances
				, const std::vector<MaterialInstance> &_submaterialInstances)
				: key(_key)
				, submeshInstances(_submeshInstances)
				, submaterialInstances(_submaterialInstances)
			{
			}

			SetRenderMode::SetRenderMode(const PrepareKey &_key
				, const RenderModeSet &_renderModes)
				: key(_key)
				, renderModes(_renderModes)
			{
			}


			CreateMesh::CreateMesh(const PrepareKey &_key)
				: key(_key)
			{}

			DeleteMesh::DeleteMesh(const PrepareKey &_key)
				: key(_key)
			{}

			CreateCamera::CreateCamera(const PrepareKey &_key)
				: key(_key)
			{}

			CreatePointLight::CreatePointLight(PrepareKey const &key)
				: key(key)
			{}

			CreateDirectionalLight::CreateDirectionalLight(PrepareKey const &key)
				: key(key)
			{}

			CreateSpotLight::CreateSpotLight(PrepareKey const &key)
				: key(key)
			{}

			SetPointLight::SetPointLight(PointLightData const &data, PrepareKey &key)
				: key(key),
				  data(data)
			{}

			SetDirectionalLight::SetDirectionalLight(DirectionalLightData const &data, PrepareKey &key)
				: key(key),
				  data(data)
			{}

			SetSpotLight::SetSpotLight(SpotLightData const &data, PrepareKey &key)
				: key(key),
				data(data)
			{}

			DeleteCamera::DeleteCamera(const PrepareKey &_key)
				: key(_key)
			{}

			DeletePointLight::DeletePointLight(const PrepareKey &_key)
				: key(_key)
			{}

			DeleteSpotLight::DeleteSpotLight(const PrepareKey &_key)
				: key(_key)
			{}

			DeleteDirectionalLight::DeleteDirectionalLight(const PrepareKey &_key)
				: key(_key)
			{}

			CameraInfos::CameraInfos(const PrepareKey &_key, CameraData const &data)
				: key(_key)
				, data(data)
			{}

			SetCurrentScene::SetCurrentScene(AScene *_scene)
				: scene(_scene)
			{
			}
		}
	}
}