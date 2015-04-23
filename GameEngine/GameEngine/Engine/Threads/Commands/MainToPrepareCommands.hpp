#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <Components/DirectionalLightComponent.hh>
#include <Components/SpotLight.hh>
#include <Components/Light.hh>
#include <Core/PrepareKey.hpp>
#include <vector>
#include <set>

namespace AGE
{
	class AScene;
	class RenderScene;
	namespace Commands
	{
		namespace MainToPrepare
		{
			struct SetTransform
			{
				SetTransform(const PrepareKey &_key, const glm::mat4 &_transform);
				PrepareKey key;
				glm::mat4 transform;
			};

			//struct SetPosition
			//{
			//	SetPosition(const PrepareKey &_key, const glm::vec3 &_position);
			//	PrepareKey key;
			//	glm::vec3 position;
			//};

			//struct SetScale
			//{
			//	SetScale(const PrepareKey &_key, const glm::vec3 &_scale);
			//	PrepareKey key;
			//	glm::vec3 scale;
			//};

			//struct SetOrientation
			//{
			//	SetOrientation(const PrepareKey &_key, const glm::quat &_orientation);
			//	PrepareKey key;
			//	glm::quat orientation;
			//};

			struct SetGeometry
			{
				SetGeometry(const PrepareKey &_key
					, const std::vector<SubMeshInstance> &_submeshInstances
					, const std::vector<MaterialInstance> &_submaterialInstances);
				PrepareKey key;
				std::vector<SubMeshInstance> submeshInstances;
				std::vector<MaterialInstance> submaterialInstances;
			};

			struct CreateMesh
			{
				CreateMesh(const PrepareKey &_key);
				PrepareKey key;
			};

			struct DeleteMesh
			{
				DeleteMesh(const PrepareKey &_key);
				PrepareKey key;
			};

			struct CreateCamera
			{
				CreateCamera(const PrepareKey &_key);
				PrepareKey key;

			};

			struct CreatePointLight
			{
				CreatePointLight(PrepareKey const &key);
				PrepareKey key;
			};

			struct CreateDirectionalLight
			{
				CreateDirectionalLight(PrepareKey const &key);
				PrepareKey key;
			};

			struct CreateSpotLight
			{
				CreateSpotLight(PrepareKey const &key);
				PrepareKey key;
			};

			struct SetPointLight
			{
				SetPointLight(glm::vec3 const &color, glm::vec3 const &attenuation, std::shared_ptr<ITexture> const &texture, PrepareKey &key);
				PrepareKey key;
				glm::vec3 color;
				glm::vec3 attenuation;
				std::shared_ptr<ITexture> texture;
			};

			struct SetDirectionalLight
			{
				SetDirectionalLight(DirectionalLightData const &data, PrepareKey &key);
				PrepareKey key;
				DirectionalLightData data;
			};

			struct SetSpotLight
			{
				SetSpotLight(glm::vec3 const &color, glm::vec3 const &direction, glm::vec3 const &range, float cutOff, std::shared_ptr<ITexture> const &texture, PrepareKey &key);
				PrepareKey key;
				glm::vec3 color;
				glm::vec3 direction;
				glm::vec3 range;
				float cutOff;
				std::shared_ptr<ITexture> texture;

			};

			struct DeleteCamera
			{
				DeleteCamera(const PrepareKey &_key);
				PrepareKey key;
			};

			struct DeletePointLight
			{
				DeletePointLight(const PrepareKey &_key);
				PrepareKey key;
			};

			struct DeleteDirectionalLight
			{
				DeleteDirectionalLight(const PrepareKey &_key);
				PrepareKey key;
			};

			struct DeleteSpotLight
			{
				DeleteSpotLight(const PrepareKey &_key);
				PrepareKey key;
			};

			struct CameraInfos
			{
				CameraInfos(const PrepareKey &_key, const glm::mat4 &_projection, RenderType pipeline);
				PrepareKey key;
				glm::mat4 projection;
				RenderType pipeline;
			};

			struct PrepareDrawLists
			{
				std::size_t alignement;
			};

			struct SetCurrentScene
			{
				SetCurrentScene(AScene *_scene);
				AScene *scene;
			};
		};
	}
}