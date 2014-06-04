#include <assimp/Importer.hpp>
#include <assimp/types.h>
#include <assimp/postprocess.h>

void main(void)
{
	Assimp::Importer importer;

	auto scene = importer.ReadFile("../../Assets/catwoman/atk close front 6.fbx", aiProcess_CalcTangentSpace |
      aiProcess_Triangulate            | 
      aiProcess_JoinIdenticalVertices  | 
      aiProcess_SortByPType);
}