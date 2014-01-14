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

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(name), CEREAL_NVP(vertices), CEREAL_NVP(normals), CEREAL_NVP(colors), CEREAL_NVP(uvs), CEREAL_NVP(indices));
		}

	};
	std::vector<Geometry> geometries;
	template <class Archive>
	void serialize(Archive &ar)
	{
//		ar(CEREAL_NVP(geometries));
	}

//	std::vector<MaterialFile> materials;
};

template <class Archive>
void serialize( Archive &ar, ObjFile::Geometry & c)
{

}

#endif   //__OBJ_FILE_HPP__