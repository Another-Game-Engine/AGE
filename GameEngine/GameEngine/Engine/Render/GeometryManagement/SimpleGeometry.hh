#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <cstdint>
#include <map>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>

namespace AGE
{
	namespace SimpleGeometry
	{
		struct SimpleGeometryKeys
		{
			AGE::Key<AGE::Vertices> verticesKey;
			AGE::Key<AGE::Painter> painterKey;
		};

		static SimpleGeometryKeys quadMesh;
		static std::map<uint32_t, SimpleGeometryKeys> icosphereMeshes;

		void generateIcoSphere(uint32_t recursion, std::vector<glm::vec3> &vertex, std::vector<unsigned int> &indices);
	}
}