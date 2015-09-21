#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <Utils/Key.hh>
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
		static SimpleGeometryKeys cubeMesh;
		static std::map<std::size_t, SimpleGeometryKeys> icosphereMeshes;

		void generateIcoSphere(std::size_t recursion, std::vector<glm::vec3> &vertex, std::vector<unsigned int> &indices);
	}
}