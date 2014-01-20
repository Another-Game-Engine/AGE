#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/AMediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>

struct ObjFile : public AMediaFile
{
	ObjFile();
	virtual ~ObjFile();

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
	virtual void _serialize(std::ofstream &os)
	{
		Archive::serialize(os, geometries.size());
		for (unsigned int i = 0, j = geometries.size(); i < j; ++i)
		{
			Archive::serialize(os, geometries[i].name);
		}
	}

	virtual void _unserialize(std::ifstream &is)
	{
		unsigned int size;
		Archive::unserialize(is, size);
		geometries.resize(size);
		for (unsigned int i = 0; i < size; ++i)
		{
			Archive::unserialize(is, geometries[i].name);
		}
	}
};

#endif   //__OBJ_FILE_HPP__