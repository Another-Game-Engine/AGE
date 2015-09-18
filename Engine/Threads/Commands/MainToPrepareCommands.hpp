#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <Components/DirectionalLightComponent.hh>
#include <Components/SpotLight.hh>
#include <Components/Light.hh>
#include <Components/CameraComponent.hpp>
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
			struct SetRenderMode
			{
				SetRenderMode(const PrepareKey &_key
					, const RenderModeSet &renderModes);
				PrepareKey key;
				RenderModeSet renderModes;
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

			struct SetDirectionalLight
			{
				SetDirectionalLight(DirectionalLightData const &data, PrepareKey &key);
				PrepareKey key;
				DirectionalLightData data;
			};

			struct SetSpotLight
			{
				SetSpotLight(SpotLightData const &data, PrepareKey &key);
				PrepareKey key;
				SpotLightData data;
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
				CameraInfos(const PrepareKey &_key, CameraData const &data);
				PrepareKey key;
				CameraData data;
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