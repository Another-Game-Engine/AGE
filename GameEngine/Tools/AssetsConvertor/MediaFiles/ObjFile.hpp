#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/AMediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <Cereal/types/base_class.hpp>

struct ObjFile : public AMediaFile
{
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
};

template <class Archive>
void serialize( Archive &ar, ObjFile & c)
{
	ar( cereal::base_class<AMediaFile>(&c), CEREAL_NVP(c.geometries));
}

template <class Archive>
void serialize( Archive &ar, ObjFile::Geometry & c)
{
	ar(CEREAL_NVP(c.name), CEREAL_NVP(c.vertices), CEREAL_NVP(c.normals), CEREAL_NVP(c.colors), CEREAL_NVP(c.uvs), CEREAL_NVP(c.indices));
}


#endif   //__OBJ_FILE_HPP__