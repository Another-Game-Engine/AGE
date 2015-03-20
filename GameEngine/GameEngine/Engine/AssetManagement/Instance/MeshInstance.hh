#pragma once

# include <string>
# include <Utils/AABoundingBox.hh>
# include <vector>
# include <Render/Key.hh>
# include <Render/GeometryManagement/Data/Vertices.hh>
# include <Render/GeometryManagement/Painting/Painter.hh>
# include <Render/Pipelining/Render/RenderModes.hh>
# include <bitset>

namespace AGE
{

	struct SubMeshInstance
	{
		Key<Painter> painter;
		Key<Vertices> vertices;
		Key<Properties> properties;
		std::bitset<RENDER_MODE_NUMBER> renderMode;
		AGE::AABoundingBox boundingBox;
		uint16_t defaultMaterialIndex;
	};

	struct MeshInstance
	{
		std::string name;
		std::string path;
		std::vector<SubMeshInstance> subMeshs;
	};

}