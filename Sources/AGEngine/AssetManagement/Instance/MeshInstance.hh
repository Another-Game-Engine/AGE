#pragma once

# include <string>
# include <Utils/AABoundingBox.hh>
# include <vector>
# include <Utils/Key.hh>
# include <Render/GeometryManagement/Data/Vertices.hh>
# include <Render/GeometryManagement/Painting/Painter.hh>
# include <Render/Pipelining/Render/RenderModes.hh>
# include <AssetManagement/Data/MeshData.hh>

namespace AGE
{
	class AssetsManager;

	struct SubMeshInstance
	{
		Key<Painter> painter;
		Key<Vertices> vertices;
		AGE::AABoundingBox boundingBox;
		uint16_t defaultMaterialIndex;
		bool isSkinned = false;
	};

	struct MeshInstance
	{
		MeshInstance()
		{
			_valid = false;
		}
		std::string name;
		std::string path;
		std::vector<SubMeshInstance> subMeshs;
		std::shared_ptr<MeshData> meshData;

		bool isValid()
		{
			return _valid;
		}
	private:
		std::atomic<bool> _valid;
		friend class AssetsManager;
	};

}