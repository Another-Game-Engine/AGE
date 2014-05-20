#pragma once

#include <fbxsdk.h>
#include <glm/glm.hpp>

namespace AGE
{
	struct FBXVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 binormal;
		glm::vec4 color;
	};

	class FBXMeshPart
	{};

	class FBXMesh
	{
	public:
		FBXMesh()
		{}

		~FBXMesh()
		{}

		// Save up data into GPU buffers.
		bool Initialize(const FbxMesh * pMesh)
		{
			bool HasNormal = pMesh->GetElementNormalCount() > 0;
			bool HasUV = pMesh->GetElementUVCount() > 0;
			bool HasTangent = pMesh->GetElementTangentCount() > 0;
			bool HasBinormal = pMesh->GetElementBinormalCount() > 0;
			bool HasColor = pMesh->GetElementVertexColorCount() > 0;

			_name = pMesh->GetNode()->GetName();
			auto materialNbr = pMesh->GetNode()->GetMaterialCount();
			if (materialNbr == 0)
				materialNbr = 1;
			_parts.resize(materialNbr);


			FbxStringList uvNameList;
			pMesh->GetUVSetNames(uvNameList);
			auto uvNumber = uvNameList.GetCount();

			int vertexIndex = 0;
			FbxVector4* controlPoints = pMesh->GetControlPoints();
			const int polygonCount = pMesh->GetPolygonCount();

			for (int polyIndex = 0; polyIndex < polygonCount; ++polyIndex)
			{
				const int polygonSize = pMesh->GetPolygonSize(polyIndex);
				for (int posInPoly = 0; posInPoly < polygonSize; ++posInPoly)
				{
					int controlPointIndex = pMesh->GetPolygonVertex(polyIndex, posInPoly);
					FBXVertex vertex;

					FbxVector4& fbxPosition = controlPoints[controlPointIndex];
					vertex.position.x = (float)fbxPosition[0];
					vertex.position.y = (float)fbxPosition[1];
					vertex.position.z = (float)fbxPosition[2];

					// Load tex coords for all uv sets
					for (int uvSetIndex = 0; uvSetIndex < uvNumber; ++uvSetIndex)
					{
						const FbxGeometryElementUV* uvElement = pMesh->GetElementUV(uvNameList.GetStringAt(uvSetIndex));
						if (uvElement)
							loadTextureCoords(pMesh, uvElement, uvSetIndex, polyIndex, posInPoly, vertexIndex, &vertex);
					}

					// Load other data
					loadNormal(fbxMesh, vertexIndex, controlPointIndex, &vertex);
					loadTangent(fbxMesh, vertexIndex, controlPointIndex, &vertex);
					loadBinormal(fbxMesh, vertexIndex, controlPointIndex, &vertex);
					loadVertexColor(fbxMesh, vertexIndex, controlPointIndex, &vertex);

					if (hasSkin)
					{
						loadBlendData(weights[controlPointIndex], &vertex);
					}

					// Determine which mesh part this vertex index should be added to based on the material that affects it.
					int meshPartIndex = 0;
					const int elementMatrialCount = fbxMesh->GetElementMaterialCount();
					for (int k = 0; k < elementMatrialCount; ++k)
					{
						FbxGeometryElementMaterial* elementMaterial = fbxMesh->GetElementMaterial(k);
						meshPartIndex = elementMaterial->GetIndexArray().GetAt(polyIndex);
					}

					// Add the vertex to the mesh if it hasn't already been added and find the vertex index.
					unsigned int index;
					if (mesh->contains(vertex))
					{
						index = mesh->getVertexIndex(vertex);
					}
					else
					{
						index = mesh->addVertex(vertex);
					}
					meshParts[meshPartIndex]->addIndex(index);
					vertexIndex++;
				}
			}


			return true;
		}

	private:
		std::string _name;
		std::vector<FBXMeshPart> _parts;
	};
}