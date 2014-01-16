#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/AMediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>

struct ObjFile : public AMediaFile
{
	ObjFile();
	virtual ~ObjFile();
	virtual void test(){}
	struct Geometry
	{
		std::string                 name;
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec4>		normals;	// normals
		std::vector<glm::vec4>		colors;		// vertices colors
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<unsigned int>	indices;	// indices
	};
	std::vector<Geometry> geometries;
	//	std::vector<MaterialFile> materials;
private:
};

#endif   //__OBJ_FILE_HPP__