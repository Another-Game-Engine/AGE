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

			SetPointLight::SetPointLight(glm::vec3 const &color, glm::vec3 const &attenuation, std::shared_ptr<ITexture> const &texture, PrepareKey &key)
				: key(key),
				color(color),
				texture(texture),
				attenuation(attenuation)
			{}

			SetDirectionalLight::SetDirectionalLight(DirectionalLightData const &data, PrepareKey &key)
				: key(key),
				  data(data)
			{}

			SetSpotLight::SetSpotLight(glm::vec3 const &color, glm::vec3 const &direction, glm::vec3 const &range, float cutOff, std::shared_ptr<ITexture> const &texture, PrepareKey &key)
				: key(key),
				color(color),
				direction(direction),
				range(range),
				cutOff(cutOff),
				texture(texture)
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

			CameraInfos::CameraInfos(const PrepareKey &_key
				, const glm::mat4 &_projection
				, RenderType pipeline)
				: key(_key)
				, projection(_projection)
				, pipeline(pipeline)
			{}

			SetCurrentScene::SetCurrentScene(AScene *_scene)
				: scene(_scene)
			{
			}
		}
	}
}