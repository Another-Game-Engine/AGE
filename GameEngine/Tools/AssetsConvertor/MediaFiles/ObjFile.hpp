#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/AMediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

struct ObjFile : public AMediaFile
{
	ObjFile() : AMediaFile()
	{
		type = typeid(ObjFile).hash_code();
		test = 42;
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
	};
	std::vector<Geometry> geometries;
	unsigned int test;
	//	std::vector<MaterialFile> materials;

	virtual AMediaFile *unserialize(cereal::JSONInputArchive &ar)
	{
		return unserialize<cereal::JSONInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::BinaryInputArchive &ar)
	{
		return unserialize<cereal::BinaryInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::XMLInputArchive &ar)
	{
		return unserialize<cereal::XMLInputArchive>(ar);
	}

	virtual void _serialize(cereal::JSONOutputArchive &ar)
	{
		ar(*this);//return serialize<cereal::JSONOutputArchive>(ar);
	}

	virtual void _serialize(cereal::BinaryOutputArchive &ar)
	{
		return serialize<cereal::BinaryOutputArchive>(ar);
	}

	virtual void _serialize(cereal::XMLOutputArchive &ar)
	{
		return serialize<cereal::XMLOutputArchive>(ar);
	}

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		ar(static_cast<ObjFile&>(*res));
		return res;
	}

	template <typename Archive>
	void serialize(Archive &ar)
	{
		ar(test);
	}
};

#endif   //__OBJ_FILE_HPP__