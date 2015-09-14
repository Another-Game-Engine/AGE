#pragma once

#include <list>
#include <memory>
#include <glm/glm.hpp>

namespace AGE
{
	struct DRBData;
	class TextureCubeMap;

	struct DRBSpotLightDrawableList
	{
		std::list<std::shared_ptr<DRBData>> meshs;
		std::shared_ptr<DRBData> spotLight;
	};

	struct CameraData
	{
		bool                            fxaa;
		bool                            bloom;
		bool                            dof;
		std::shared_ptr<TextureCubeMap> texture;
		glm::mat4                       projection;
		unsigned int                    pipeline;

		CameraData(CameraData const &cam);
		CameraData();
		CameraData &operator=(CameraData const &cam);
	};

	struct CameraInfos
	{
		glm::mat4 view;
		CameraData data;
	};

	struct DRBCameraDrawableList
	{
		std::list<std::shared_ptr<DRBData>> meshs;
		std::list<std::shared_ptr<DRBData>> pointLights;
		std::list<std::shared_ptr<DRBSpotLightDrawableList>> spotLights;
		CameraInfos cameraInfos;
	};

	struct DRBCameraDrawableListCommand
	{
		DRBCameraDrawableListCommand(std::shared_ptr<DRBCameraDrawableList> _list);
		DRBCameraDrawableListCommand(DRBCameraDrawableListCommand &&o);
		std::shared_ptr<DRBCameraDrawableList> list;
	};
}