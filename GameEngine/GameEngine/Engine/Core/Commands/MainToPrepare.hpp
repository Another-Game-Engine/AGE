#ifndef __COMMANDS_MAIN_TO_PREPARE__
#define __COMMANDS_MAIN_TO_PREPARE__

#include <glm/glm.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Core/PrepareKey.hpp>
#include <glm/glm.hpp>
#include <Geometry/Material.hpp>
#include <Geometry/Mesh.hpp>
#include <Core/CullableObjects.hh>

class AScene;

namespace AGE
{
	class RenderScene;
	namespace Commands
	{
		namespace MainToPrepare
		{
			struct SetPosition
			{
				SetPosition(const PrepareKey &_key, const glm::vec3 &_position);
				PrepareKey key;
				glm::vec3 position;
			};

			struct SetScale
			{
				SetScale(const PrepareKey &_key, const glm::vec3 &_scale);
				PrepareKey key;
				glm::vec3 scale;
			};

			struct SetOrientation
			{
				SetOrientation(const PrepareKey &_key, const glm::quat &_orientation);
				PrepareKey key;
				glm::quat orientation;
			};

			struct SetGeometry
			{
				SetGeometry(const PrepareKey &_key
					, const AGE::Vector<SubMeshInstance> &_submeshInstances
					, const AGE::Vector<MaterialInstance> &_materialInstances
					, const gl::Key<AnimationInstance> &_animation);
				PrepareKey key;
				AGE::Vector<SubMeshInstance> submeshInstances;
				AGE::Vector<MaterialInstance> materialInstances;
				gl::Key<AnimationInstance> animation;
			};

			struct CreateDrawable
			{
				CreateDrawable(const PrepareKey &_key);
				PrepareKey key;
			};

			struct DeleteDrawable
			{
				DeleteDrawable(const PrepareKey &_key);
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

			struct SetPointLight
			{
				SetPointLight(glm::vec3 const &color, glm::vec3 const &range, PrepareKey &key);
				PrepareKey key;
				glm::vec3 color;
				glm::vec3 range;
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

			struct CameraInfos
			{
				CameraInfos(const PrepareKey &_key, const glm::mat4 &_projection);
				PrepareKey key;
				glm::mat4 projection;
			};

			struct PrepareDrawLists
			{
			};

			struct SceneUpdateBegin
			{
				SceneUpdateBegin(std::size_t _sceneId);
				std::size_t sceneId;
			};
		};
	}
}
#endif