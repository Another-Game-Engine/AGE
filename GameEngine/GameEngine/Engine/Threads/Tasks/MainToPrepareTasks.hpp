#ifndef __TASKS_MAIN_TO_PREPARE__
#define __TASKS_MAIN_TO_PREPARE__

#include <TMQ/message.hpp>
#include <AssetManagement/Data/MaterialData.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>

namespace AGE
{
	class AScene;
	namespace Tasks
	{
		namespace MainToPrepare
		{
			struct CreateScene : public TMQ::FutureData<bool>
			{
				CreateScene(AScene *_scene);
				AScene *scene;
			};

			struct AddMaterial : TMQ::FutureData < MaterialInstance >
			{
				MaterialData data;
				AddMaterial(MaterialData const &pData) :
					data(pData) {}
			};

			struct SetMeshMaterial
			{
				std::shared_ptr<MaterialSetInstance> material;
				std::shared_ptr<MeshInstance> mesh;

				SetMeshMaterial(std::shared_ptr<MaterialSetInstance> pMaterial,
					std::shared_ptr<MeshInstance> pMesh) :
					material(pMaterial),
					mesh(pMesh)
				{ }

			};
		}

	}
}
#endif