#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>

struct ObjFile : public MediaFile<ObjFile>
{
	ObjFile() : MediaFile<ObjFile>()
	{
	}
	virtual ~ObjFile(){}
	struct Geometry
	{
		std::string                 name;
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec4>		normals;	// normals
		std::vector<glm::vec4>		colors;		// vertices colors
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<unsigned int>	indices;	// indices

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(name,vertices, normals,colors,uvs, indices);
		}
	};

	std::vector<Geometry> geometries;
	std::shared_ptr<AMediaFile> material;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		res->manager = manager;
		ar(static_cast<ObjFile&>(*res));
		return res;
	}

	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(CEREAL_NVP(geometries));
		ar(cereal::make_nvp("material", material->name));
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		ar(geometries);
		std::string matName;
		ar(matName);
		if (matName != "NULL")
			manager->assetHandle(matName, &material);
	}

};

#endif   //__OBJ_FILE_HPP__